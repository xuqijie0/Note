## LogLevel

```c++
typedef enum {
    LTrace = 0, LDebug, LInfo, LWarn, LError
} LogLevel;
```

- `LTrace`：跟踪级别
- `LDebug`：调试级别
- `LInfo`：信息级别
- `LWarn`：警告级别
- `LError`：错误级别

## Logger

`LogContextPtr  shared_ptr<LogContext>`:指向LogContext的智能指针

`Ptr   shared_ptr<Logger>`

### 成员变量

`LogContextPtr _last_log`:

`string _logger_name`:	logger名称

`shared_ptr<LogWriter> _writer`: 写日志器

`shared_ptr<LogChannel> _default_channel`: 初始为 ConsoleChannel

`map<string, shared_ptr<LogChannel> > _channels`:存储 LogChannel



### `Logger& Instance()` 

获取日志单例

### add

添加日志通道

```c++
add(const std::shared_ptr<LogChannel>& channel)
{
	_channels[channel->name()] = channel;
}
```

### del

删除日志通道, 通过名字进行删除

```c++
del(const std::string name)
{
	_channels.erase(name);
}
```

### get

通过名字获取日志通道

```c++
shared_ptr<LogChannel> get(const std::string name)
{
	auto it = _channels.find(name);
	if (it == _channels.end()) {
		return nullptr;
	}
	return it->second;
}
```

### setWriter

设置写log器

```c++
void Logger::setWriter(const std::shared_ptr<LogWriter>& writer)
{
	_writer = writer;
}
```

### setLevel

设置所有日志通道的日志等级

```c++
void Logger::setLevel(LogLevel level)
{
	for (auto& chn : _channels) {
		chn.second->setLevel(level);
	}
}
```

### getName

获取logger名称

```c++
const std::string& Logger::getName() const
{
	return _logger_name;
}
```

### write

写日志

```C++
void Logger::write(const LogContextPtr& ctx)
{
	if (_writer) {
		_writer->write(ctx, *this);
	}
	else {
		writeChannels(ctx);
	}
}
```

### writeChannels

写日志到channel，仅供AsyncLogWriter调用

### writeChannels_l



```c++
void Logger::writeChannels_l(const LogContextPtr& ctx)
{
	if (_channels.empty()) {
		_default_channel->write(*this, ctx);
	}
	else {
		for (auto& chn : _channels) {
			chn.second->write(*this, ctx);
		}
	}
	_last_log = ctx;
	_last_log->_repeat = 0;
}
```



## LogContext

**继承 `ostringstream`**，用户待输出的日志信息会先存储在ostringstream中。

用于存储日志信息，包括日志级别、日志所在文件名、函数名、行号、用户待输出的日志信息。

### 成员变量

`level : LogLevel`  日志等级

`_repeat : int` 日志重复次数

`_line : int` 行号（由\__LINE__获得）

`_file : string` 文件名(由\__FILE__获得)

`_function : string` 函数名(由\__FUNCTION__获得)

`_thread_name : string` 

`_moudle_name : string` 

`_flag : string` 

`_tv : struct timeval`

私有变量：

`_got_content : bool`

`_content : string` 存储ostringstream中的内容

### str

`str()` 将ostringstream中的内容赋给`_content`, 再返回`_content` 

```c++
const std::string& LogContext::str()
{
	if (_got_content) {
		return _content;
	}
	_content = std::ostringstream::str();
	_got_content = true;
	return _content;
}
```



## LogContextCapture 

日志上下文捕获器

### 成员变量

`_ctx : LogContextPtr` 指向LogContext的智能指针

`_logger : Logger&` Logger类的引用

### 构造/析构函数

### 重载 <<

+ `LogContextCapture &operator<<(std::ostream &(*f)(std::ostream &));`

  >重载了输出运算符，遇到`endl`立即输出日志

  ```C++
  LogContextCapture& LogContextCapture::operator<<(std::ostream& (*f)(std::ostream&))
  {
  	if (!_ctx) {
  		return *this;
  	}
  	_logger.write(_ctx);
  	_ctx.reset();
  	return *this;
  }
  ```

+ 将用户输入的log信息存储到LogContext对象中


  ```C++
  template<typename T>
  		LogContextCapture& operator<< (T&& data) {
  			if (!_ctx) {
  				return *this;
  			}
  			(*_ctx) << std::forward<T>(data);
  			return *this;
  		}
  ```



## LogWriter

写日志器

### 写日志 纯虚函数

`virtual void write(const LogContextPtr& ctx, Logger& logger) = 0`



## AsyncLogWriter

继承`LogWriter` ,写日志器

开启一个线程，先将日志存储在队列中，然后异步的将队列中的日志信息输出。

### 构造函数

开启一个线程，运行`run`方法

### 成员变量

+ `flushAll` 将日志信息输出到channels
+ `run` 调用`flushAll` 处理日志子信息
+ `write` 添加写事件，唤醒`run`的等待
+ `_exit_flag : bool`
+ `_sem : semaphore`
+ `_mutex : mutex`
+ `_thread : shared_ptr<thread> ` 用于执行`run`的线程
+ `_pending : List<pair<LogContextPtr, Logger*> >`

### write

添加写事件至 _pending ，并唤醒`run`等待

```c++
void AsyncLogWriter::write(const LogContextPtr& ctx, Logger& logger)
{
	{
		std::lock_guard<std::mutex> lock(_mutex);
		_pending.emplace_back(std::make_pair(ctx, &logger));
	}
	_sem.post();
}
```



### flushAll

将现有日志信息输出到channels

```c++
void AsyncLogWriter::flushAll()
{
	//	创建临时变量 tmp， 类型与_pending相同
	decltype(_pending) tmp;
	//	此处大括号用于创建一个作用域，限定lock_guard对象的生命周期
	//	当程序执行到大括号结束，luck_guard对象被销毁，自动释放互斥锁的持有，解锁_mutex
	{
		//	使用lock_guard加锁，确保互斥访问_pending
		std::lock_guard<std::mutex> lock(_mutex);
		//	将_pending 内容与tmp交换，同时清空_pending
		tmp.swap(_pending);
	}
	//	对temp中的每一个元素执行操作
	tmp.for_each([&](std::pair<LogContextPtr, Logger*>& pr) {
		pr.second->writeChannels(pr.first);
	});
}
```

### run

线程主循环，用于异步日志写入的后台线程

```c++
void AsyncLogWriter::run()
{
	setThreadName("async log");
	while (!_exit_flag) {
		_sem.wait();
		flushAll();
	}
}
```



## LogChannel

### 成员变量

+ `_name : string`	派生类channel的名字
+ `_level : LogLevel`

`name` :

```c++
const std::string& LogChannel::name() const
{
	return _name;
}
```

`setLevel` :

```c++
void LogChannel::setLevel(LogLevel level)
{
	_level = level;
}
```

### `write`

纯虚函数

### `printTime`:

```C++
std::string LogChannel::printTime(const timeval& tv)
{
	//	本地时间
	auto tm = getLocalTime(tv.tv_sec);
	char buf[128];
	snprintf(buf, sizeof(buf), "%d-%02d-%02d %02d:%02d:%02d.%03d",
		1900 + tm.tm_year,
		1 + tm.tm_mon,
		tm.tm_mday,
		tm.tm_hour,
		tm.tm_min,
		tm.tm_sec,
		(int)(tv.tv_usec / 1000));
	return buf;
}
```

### `format` : 

**虚函数**

打印日志至输出流   更改字体颜色，打印函数名、源文件名、源码行等细节

## FileChannelBase

继承`LogChannel`，输出日志到文件

### 成员函数

`virtual bool open();`：创建并打开_path路径下的文件

`std::string _path;` 文件路径

`std::ofstream _fstream;` 文件流

## ConsoleChannel

### `write`

将日志写到终端

## FileChannelBase

## 成员变量

+ `_path : string`
+ `_fstream : ofstream`

### `size`

### `close`

关闭_fstream文件流

### `open`

打开_path所指文件

### `setPath`

设置变量_path,并打开其所指文件

### `write`

将日志信息写入文件

```c++
void FileChannelBase::write(const Logger& logger, const LogContextPtr& ctx)
{
	if (_level > ctx->_level) {
		return;
	}
	if (!_fstream.is_open()) {
		open();
	}
	//	打印至文件，不启用颜色
	format(logger, _fstream, ctx, false);
}
```



## FileChannel

自动清理日志文件 ，继承`FileChannelBase`

### 成员变量

```c++
size_t _log_max_day;	//	最多保存多少天
size_t _log_max_size;	//	日志文件最大大小
size_t _log_max_count;	//	日志文件最多个数
size_t _index;			//	当前日志切片文件索引
//	存放日志名，用于记录日志
std::set<std::string> _log_file_map;
```

### 成员函数

`getLogFilePath`:根据GMT UNIX时间戳生成日志文件名

`getLogFileTime`：根据日志文件名返回时间戳

`changeFile`：创建并且换到下一个日志切片文件

`clean()` ：删除日志切片文件，条件为超过最大保存天数与最大切片个数