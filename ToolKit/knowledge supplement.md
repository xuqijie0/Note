# 语法

###      \## 与  \#

> + \## 运算符：用于将两个标识符连接成一个新的标识符。
>   例如，如果有两个宏参数x和y，则可以使用\##运算符将它们连接在一起，行成一个新的标识符
>
>   ```C++
>   #define CONCAT(x, y) x ## y
>     
>   int main() {
>       int xy = 10;
>       int result = CONCAT(x, y);  // 相当于 int result = xy;
>       return 0;
>   }
>   ```
>
>   在上述示例中，``CONTACT(x, y)``宏将参数x和y使用##运算符连接在一起，形成新的标识符`xy`
>
> + \# 运算符：用于将宏参数转换为字符串字面量。它在宏定义中用于字符串化操作。
>   ```C++
>   #include <iostream>
>     
>   #define STR(x) #x
>     
>   int main() {
>       int value = 42;
>       std::cout << STR(value) << std::endl;  // 输出 "value"
>       return 0;
>   }
>   ```
>
>   在上述示例中，`STR(value)`宏将参数`value`转换为字符串字面量，即`"value"`

总结：

- `##` 运算符用于标识符连接，将两个标识符连接成一个新的标识符。
- `#` 运算符用于字符串化，将宏参数转换为字符串字面量。

### string 成员函数`.data()`

> `.data()` 是 C++ 中 `std::string` 类的成员函数，用于获取字符串的底层字符数组（C 风格字符串）的指针。

### std::flush

>流操作符，用于强制将流缓冲区中的数据立即刷新到输出设备。
>
>在 C++ 中，输出流通常具有一个缓冲区，它用于临时存储要输出的数据。缓冲区的目的是减少对输出设备的频繁访问，从而提高效率。默认情况下，当遇到换行符 `\n` 或缓冲区已满时，输出流会自动刷新缓冲区。

### std::ostream成员函数`.tellp()`

>返回当前文件流的写入位置（指针位置）

### strptime

> strptime是一个C函数，用于将字符串解析为日期和时间的结构。他接受两个参数：要解析的字符串和日期时间格式。他将解析后的结果存储在提供的日期时间结构中。
>
> ```c++
> #include <time.h>
> 
> char* strptime(const char* str, const char* format, struct tm* tm);
> ```
>
> - 参数 `str` 是要解析的字符串。
> - 参数 `format` 是日期时间的格式字符串，用于指定如何解析输入字符串。格式字符串的语法与 `strftime` 函数的格式相似。
> - 参数 `tm` 是指向 `struct tm` 结构的指针，用于存储解析后的日期和时间。
>
> 函数返回一个指向未解析部分的指针，或者如果解析失败，则返回空指针。
>
> ```C++
> #include <iostream>
> #include <ctime>
> 
> int main() {
>     const char* input = "2024-05-07 10:30:00";
>     const char* format = "%Y-%m-%d %H:%M:%S";
> 
>     struct tm tm;
>     if (strptime(input, format, &tm) != nullptr) {
>         std::cout << "Year: " << tm.tm_year + 1900 << std::endl;
>         std::cout << "Month: " << tm.tm_mon + 1 << std::endl;
>         std::cout << "Day: " << tm.tm_mday << std::endl;
>         std::cout << "Hour: " << tm.tm_hour << std::endl;
>         std::cout << "Minute: " << tm.tm_min << std::endl;
>         std::cout << "Second: " << tm.tm_sec << std::endl;
>     }
> 
>     return 0;
> }
> ```

### time(nullptr)

> `time(nullptr)` 是一个C++ 11 中的方法，用于获取当前的UNIX时间戳。它返回一个time_t类型的值，表示从1970年1月1日00：00：00 UTC（协调世界时）到当前时间点的秒数。
>
> ```c++
> #include <iostream>
> #include <ctime>
> 
> int main() {
>     time_t timestamp = time(nullptr);
>     std::cout << "Current UNIX timestamp: " << timestamp << std::endl;
> 
>     return 0;
> }
> ```

### snprintf

> `snprintf` 是一个 C 函数，用于将格式化的数据写入字符串缓冲区。
>
> ```C++
> #include <cstdio>
> 
> int snprintf(char* str, size_t size, const char* format, ...);
> ```
>
> - 参数 `str` 是一个指向目标字符串缓冲区的指针，用于接收格式化后的数据。
> - 参数 `size` 是目标缓冲区的大小，包括终止的空字符。
> - 参数 `format` 是一个 C 字符串，包含了格式化的指令，指示如何将数据写入缓冲区。
> - `...` 表示可变数量的参数，用于提供要格式化的数据。函数返回实际写入缓冲区的字符数（不包括终止的空字符），或者如果失败则返回一个负值。
>
> 函数返回实际写入缓冲区的字符数（不包括终止的空字符），或者如果失败则返回一个负值。

### strrchr

> 用于在一个 C 字符串中查找指定字符的最后一次出现的位置
>
> `auto pos = strrchr(file, '/');`

# 同步与异步

## `recursive_mutex`

> `recursive_mutex` 是 C++ 标准库中提供的一种线程同步原语,用于实现可重入的互斥锁。它与标准的 `mutex` 类型非常相似,但有以下重要区别:
>
> 1. **可重入性**: 在同一线程中,一个线程可以多次对同一个 `recursive_mutex` 进行加锁,而不会阻塞自己。这在某些情况下很有用,例如在函数调用中需要再次获取同一个互斥锁。
> 2. **内部计数**: `recursive_mutex` 维护了一个内部计数器,记录当前线程已经获取了多少次锁。当计数器归零时,互斥锁才会被完全释放。
>
> ```c++
> #include <iostream>
> #include <thread>
> #include <mutex>
> 
> std::recursive_mutex rm;
> 
> void worker() {
>     rm.lock();
>     std::cout << "Thread ID: " << std::this_thread::get_id() << " - Lock acquired" << std::endl;
> 
>     // Do some work
>     rm.lock(); // This won't block since the same thread holds the lock
>     std::cout << "Thread ID: " << std::this_thread::get_id() << " - Acquired lock again" << std::endl;
>     rm.unlock();
> 
>     std::cout << "Thread ID: " << std::this_thread::get_id() << " - Unlocking" << std::endl;
>     rm.unlock();
> }
> 
> int main() {
>     std::thread t1(worker);
>     std::thread t2(worker);
> 
>     t1.join();
>     t2.join();
> 
>     return 0;
> }
> ```
>
> 在这个例子中:
>
> 1. 两个线程 `t1` 和 `t2` 都调用 `worker()` 函数,该函数使用了 `recursive_mutex` `rm`。
> 2. 在 `worker()` 函数中,线程首先获取 `rm` 的锁,然后再次尝试获取同一个锁,这不会导致阻塞,因为该线程已经持有该锁。
> 3. 在完成工作后,线程依次解锁 `rm`。
>
> 与标准的 `mutex` 不同，`recursive_mutex` 允许同一线程多次获取同一个锁,而不会发生死锁。这在某些需要嵌套锁定的情况下非常有用,例如函数递归调用或事务处理等。
>
> 使用 `recursive_mutex` 时需要注意的是,它会增加内存和处理开销,因此应该仅在确实需要可重入性的情况下使用。在大多数情况下,使用标准的 `mutex` 就足够了。s

# Windows

### 句柄

> [深入了解Windows句柄到底是什么](https://blog.csdn.net/wenzhou1219/article/details/17659485)

### FindFirstFileA

> `FindFirstFileA` 是一个 Windows API 函数，用于在指定目录中匹配指定文件名模式的第一个文件或目录。它是针对ANSI字符集的版本，接受以ANSI字符串形式表示的目录路径或文件名模式。
>
> ```c++
> HANDLE FindFirstFileA(
>     LPCSTR            lpFileName,
>     LPWIN32_FIND_DATAA lpFindFileData
> );
> ```
>
> - `lpFileName`：指向以 ANSI 字符串形式表示的目录路径和文件名模式的指针。可以使用通配符 * 和 ? 进行模式匹配。
> - `lpFindFileData`：指向 `WIN32_FIND_DATAA` 结构体的指针，用于接收找到的文件或目录的信息。
>
> `FindFirstFileA` 函数返回一个句柄（`HANDLE`），用于后续的文件搜索操作。如果函数成功执行，将返回指向找到的第一个文件或目录的句柄。如果函数执行失败，返回值将为 `INVALID_HANDLE_VALUE`，可以使用 `GetLastError` 函数获取错误码。

### FindClose

> `FindClose` 是一个 Windows API 函数，用于关闭使用 `FindFirstFile` 或 `FindFirstFileEx` 函数打开的文件搜索句柄。
>
> ```c++
> BOOL FindClose(
>     HANDLE hFindFile
> );
> ```
>
> - `hFindFile`：要关闭的文件搜索句柄。
>
> `FindClose` 函数用于释放文件搜索句柄所占用的系统资源。在使用 `FindFirstFile` 或 `FindFirstFileEx` 函数获取文件搜索句柄后，当不再需要该句柄时，应调用 `FindClose` 函数进行关闭。
>
> 以下是一个示例，演示了如何使用 `FindFirstFile`、`FindNextFile` 和 `FindClose` 函数来遍历目录中的文件：
>
> ```c++
> #include <windows.h>
> #include <iostream>
> 
> int main() {
>     const wchar_t* path = L"C:\\path\\to\\directory\\*";
>     WIN32_FIND_DATAW findData;
> 
>     HANDLE hFind = FindFirstFileW(path, &findData);
>     if (hFind != INVALID_HANDLE_VALUE) {
>         do {
>             if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
>                 std::wcout << L"File Name: " << findData.cFileName << std::endl;
>                 std::wcout << L"File Size: " << findData.nFileSizeLow << L" bytes" << std::endl;
>                 std::wcout << std::endl;
>             }
>         } while (FindNextFileW(hFind, &findData));
> 
>         FindClose(hFind);
>     } else {
>         std::cout << "Failed to find files." << std::endl;
>     }
> 
>     return 0;
> }
> ```
>
> 在上述示例中，我们使用 `FindFirstFileW` 函数获取第一个文件的搜索句柄，并使用 `do-while` 循环和 `FindNextFileW` 函数遍历剩余的文件。在循环中，我们判断文件的属性，如果不是目录，就打印文件名和文件大小。
>
> 最后，我们使用 `FindClose` 函数关闭文件搜索句柄，以释放资源。

### vsprintf_s

> Microsoft Visual Studio C/C++ 编译器提供的函数之一，位于 `<stdio.h>` 头文件中。
>
> `vsprintf_s` 函数接受一个指向缓冲区的指针、缓冲区大小、格式化字符串和一个 `va_list` 类型的参数列表。它根据格式化字符串和可变参数将生成的字符串写入缓冲区。
>
> ```c++
> #include <stdio.h>
> 
> int vsprintf_s(char *buffer, size_t sizeOfBuffer, const char *format, va_list argptr);
> ```
>
> 参数说明：
>
> - `buffer`：指向目标缓冲区的指针，用于存储生成的字符串。
> - `sizeOfBuffer`：目标缓冲区的大小。
> - `format`：格式化字符串，指定了生成字符串的格式。
> - `argptr`：`va_list` 类型的参数列表，包含了可变参数的信息。
>
> 返回值：
>
> - 如果成功将生成的字符串写入缓冲区，则返回写入的字符数（不包括终止符 `\0`）。
> - 如果发生错误，则返回负值。