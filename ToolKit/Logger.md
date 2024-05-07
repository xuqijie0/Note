## Logger

## LogContext

### 成员函数

`str()` 获取成员变量`_content` 的内容，返回string

## LogContextCapture 

日志上下文捕获器

### 构造/析构函数

### 重载 <<

+ `LogContextCapture &operator<<(std::ostream &(*f)(std::ostream &));`

  >重载了输出运算符，遇到`endl`立即输出日志



## AsyncLogWriter

继承`LogWriter` ,写日志器

开启一个线程，先将日志存储在队列中，然后异步的将队列中的日志信息输出。

### 构造函数

开启一个线程，运行`run`方法

### 成员函数/变量

+ `flushAll` 将日志信息输出到channels
+ `run` 调用`flushAll` 处理日志子信息
+ `write` 添加写事件，唤醒`run`的等待

## LogChannel

### 成员函数

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

`printTime`:

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

`format` : 打印日志至输出流   更改字体颜色，打印函数名、源文件名、源码行等细节

## FileChannelBase

### 成员函数

`virtual bool open();`：创建并打开_path路径下的文件

`std::string _path;` 文件路径

`std::ofstream _fstream;` 文件流