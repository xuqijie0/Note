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

# Windows

+ 句柄
  [深入了解Windows句柄到底是什么](https://blog.csdn.net/wenzhou1219/article/details/17659485)

