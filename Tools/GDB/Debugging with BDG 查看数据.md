# Debugging with GDB: 查看数据

[TOC]

## 程序变量

在C/C++中，变量遵守作用域规则，有不同的生存期、链接类型。不同的函数中可能有相同的变量名，不同的文件中也可能有相同的文件名，必须无歧义地向GDB制定变量名。

### 具有外部链接的符号

非static的全局变量具有外部链接，全局只有一个，所以可以直接使用`print`命令打印。

比如打印全局变量`global`

```
p global
```

### 具有内部链接的符号

#### 非static局部变量

非static局部变量只在作用域内可见、生存期也只在作用域内，只有变量在作用域内时才能使用GDB查看。

同名局部变量可以出现在不同函数中，因此在查看局部变量时要指定栈帧，如果不指定就使用当前栈帧。

指定变量的方法为：

- `function::variable`
- 使用`frame`命令切换到变量查看栈帧，再使用`print`

比如以下程序`varaible.c`：

```C
void func2(int x)
{
	x = 100;
}

void func1(int x) {
	x += 5;
	func2(x);
}

int 
main(int argc, char *argv[])
{
	int x = 20;
	func1(x);
	return 0;
}


```

编译：

```
gcc -O0 -ggdb -o variable variable.c
```

使用GDB调试，在进入`func1`和`main`中的变量`x`

```
Breakpoint 1, func1 (x=20) at /home/GDB/variable.c:7
7               x += 5;
(gdb) n
8               func2(x);
(gdb) p x
$2 = 25
(gdb) p main::x
$3 = 20
(gdb) bt
#0  func1 (x=25) at /home/kongjun/c_practice/variable.c:8
#1  0x0000555555555172 in main (argc=1, argv=0x7fffffffdfc8) at /home/GDB/variable.c:15
(gdb) f 1
#1  0x0000555555555172 in main (argc=1, argv=0x7fffffffdfc8) at /home/GDB/variable.c:15
15              func1(x);
(gdb) p x
$4 = 20

```

**要避免在刚进入栈帧或即将退出栈帧时查看非static局部变量**。在机器指令层面，建立栈帧和初始化变量一般由多条指令完成，如果在这时查看变量，很可能变量的值还没有被设置好，查看到错误的值，退出栈帧时同理。

#### static变量

static的全局变量和局部变量具有内部链接，一般存储在`.data`段或`.bss`段中。虽然static变量也只在作用域内可见，但是却存在于整个程序运行期间，鉴于这个特性，GDB允许在static局部变量所在作用域内查看变量。

不同文件中可能有同名的static变量，所以必须指明变量所在的文件或函数。指示变量位置的方法如下：

- `function::variable`
- `file::variable`或`'file'::variable`

使用`'file'::variable`可以避免文件名和函数名冲突的情况。

### 注意事项

在编译程序时，编译器很可能会对变量或它所在的函数进行优化，比如消除不必要的函数调用、将循环，等，这是可能无法查看到变量真正的值，甚至无法查看到变量。上面的`variable.c`就是一个例子，变量`x`并没有真的被使用，在使用O2级别的优化编译时，变量`x`和函数`func1`、`func2`会被直接消除，`main`直接返回。

如果要查看变量的真实值，要么禁止优化，要么使用某些支持查看变量真实值的调试格式。

在使用`::`记号时要小心和C++中的作用域运算符冲突，如果真的发生了冲突，GDB优先使用C++作用域运算符的语义。

## 查看数组的值

对于数组类型，GDB分析它的符号信息后获取了它的起始地址、长度，可以直接使用`print`命令查看。某些指针指向某个元素，但它实际上代表一个数组，GDB不知道它代表一个数组，无法直接使用`print`命令查看，这时需要我们手动地将其转换为数组。方法如下：

- 使用类型转换
- `element@len`：`element`代表数组第一个元素，`@len`代表数组元素个数。

`arry.c`：

```c
int 
main(int argc, char *argv[])
{
	int arry[] = {1, 2, 3, 4, 5, 6};
	int *p = arry;

	return 0;
}

```

使用GDB调试：

```
(gdb) n
6               int *p = arry;
(gdb) p arry
$2 = {1, 2, 3, 4, 5, 6}
(gdb) n
8               return 0;
(gdb) p p
$3 = (int *) 0x7fffffffdec0
(gdb) p *p
$4 = 1
(gdb) p *p@6
$5 = {1, 2, 3, 4, 5, 6}
(gdb) p *p@9
$6 = {1, 2, 3, 4, 5, 6, -8512, 32767, 1431654768}

```

指针`p`指向数组第一个元素，实际上代表数组`arry`，但是被指向对象的类型却是`int`，GDB无法知道它代表数组。

## 查看/查找内存

查看内存指令为`examine`(`x`)。格式为：`x /nfu`，其中`n`是要重复的次数，`f`是打印格式，`u`是单元大小。具体的值参考[Examining data](https://sourceware.org/gdb/current/onlinedocs/gdb/Memory.html#Memory)。

GDB 除了查看内存，还提供了在内存中查找某个值的命令`find`。语法为：

```
find [/sn] start_addr, +len, val1 [, val2, …]
find [/sn] start_addr, end_addr, val1 [, val2, …]
```

其中`s`是查找的变量的大小(b,h,w,g)，`n`是匹配的最大个数。`find`命令可以根据查找的变量类型自动判断大小，因此不需要指定`s`，直接`find`即可，这样可以实现查找不同大小的变量。

GDB 提供了变量`$_`,可以通过它访问到`find`匹配到的最后一个地址。

程序如下：

```c
void
hello ()
{
  static char hello[] = "hello-hello";
  static struct { char c; short s; int i; }
    __attribute__ ((packed)) mixed
    = { 'c', 0x1234, 0x87654321 };
  printf ("%s\n", hello);
}
```

GDB 调试：

```gdb
(gdb) find &hello[0], +sizeof(hello), "hello"
0x804956d <hello.1620+6>
1 pattern found
(gdb) find &hello[0], +sizeof(hello), 'h', 'e', 'l', 'l', 'o'
0x8049567 <hello.1620>
0x804956d <hello.1620+6>
2 patterns found.
(gdb) find &hello[0], +sizeof(hello), {char[5]}"hello"
0x8049567 <hello.1620>
0x804956d <hello.1620+6>
2 patterns found.
(gdb) find /b1 &hello[0], +sizeof(hello), 'h', 0x65, 'l'
0x8049567 <hello.1620>
1 pattern found
(gdb) find &mixed, +sizeof(mixed), (char) 'c', (short) 0x1234, (int) 0x87654321
0x8049560 <mixed.1625>
1 pattern found
(gdb) print $numfound
$1 = 1
(gdb) print $_
$2 = (void *) 0x8049560
```



## 变量历史

​	每次使用`print`打印变量的值，都会将该次打印的值记录在变量历史中，每次打印都有一个编号，这就是为什么`print`会输出类似`$1 = 100`的原因，其中`$1`中的`1`就是变量历史编号。

可以使用`show variables`查看近 10 次变量历史，`show variables n`查看以历史编号 n 为中心的变量历史，`show variables +`打印上次`print`之前的 10 此变量历史。

也可以使用`$`、`$$`、`$N`或`$N`来打印变量历史中的值。`$`表示最近一次`print`，`$$`表示上上次`print`，`$N`表示编号为`N`的变量历史，`$$N`表示从上次`print`开始的变量历史号。如`$$0`等同于`$`（上次`print`），`$$2`前 3 次`print`。

## convenience variable

convenience variable 主要有两个用途：

- 访问某些 GDB 记录的信息
- 定义变量利用 GDB 命令实现自己需要的功能

先介绍 GDB 记录的信息:

- `$_exitsignal`: 杀死进程的信号。函数`$_isvoid()`可以判断进程是否被信号杀死。

- `$_exitcode`: 进程退出码

- `$_thread`: 进程编号
- `$_gthread`: 全局进程编号

还有其他一些变量，可以参考 GDB 使用手册。

还可以利用 convenience variable 自动化一些操作。比如有一个指针数组，其中的指针指向对象，现在需要查看数组中指针指向的对象，使用 convenience variable 可以轻易完成这个任务：

```gdb
set $i = 0
p *array[$i++]
```

然后按不断按回车即可遍历数组并查看指针指向的对象。

## 寄存器

`info registers`查看通用寄存器，`info all-registers`查看包括浮点数寄存器在内的寄存器。

`print $<register>`可以查看寄存器`register`的值，如访问 RISCV 寄存器 a0：`print $a0`。

## 将内存/变量拷贝到文件中

有时可能想要比较多次运行过程中某变量或某块内存的值，GDB 没有直接提供这种功能，这时可以将内存/变量拷贝到文件中再比较。

```gdb
dump [format] memory filename start_addr end_addr
dump [format] value filename expr
append [binary] memory filename start_addr end_addr
append [binary] value filename expr
restore filename [binary] bias start end
```

`dump`命令将内存/变量写入到文件`filename`中，`append`将内存/变量附加到文件中，`restore`命令将文件中的内容恢复到内存中。

文件格式必须是以下之一：

- `binary`

  Raw binary form.

- `ihex`

  Intel hex format.

- `srec`

  Motorola S-record format.

- `tekhex`

  Tektronix Hex format.

- `verilog`

  Verilog Hex format.

如果不指定格式，`dump`和`append`默认写入 binary 数据，实际上`append`暂时只支持 binary。`restore`可以自动判断格式，但由于 binary 无格式，必须手动指定。其他几种格式文件中以及记录了固定的地址，`restore`时不需要指定`bias`，binary 文件地址总是从 0 开始，因此`restore filename binary bias start end`实际上会将文件中的内容写入进程地址`bias`。

## 生成 core file

`generate-core-file [file]`或`gcore [file]`生成 core dump，如果不指定文件名，生成的文件名为`core.pid`。

## 改善 GDB 的输出

GDB 默认`print`输出是最简的，部分内容不会被打印，并且没有格式化。可以通过设置一定的选项，改善输出，这里列出几个我个人觉得比较有用的选项：

- `set print object on`: 当打印指向对象的指针时，显示其真实类型
- `set print array on`: 用更好的格式打印数组，但是需要占用更多空间
- `set print pretty on`: 打印结构体/类时使用缩进
- `set print vtbl on`: 打印 C++ 虚函数表



