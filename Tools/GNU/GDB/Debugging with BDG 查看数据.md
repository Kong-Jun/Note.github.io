# Debugging with BDG: 查看数据

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



