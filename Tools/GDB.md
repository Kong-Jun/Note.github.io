[TOC]
## 运行和停止程序
### 使用GDB
使用GDB调试的程序必须具有调试标志,因此在使用GCC编译程序时应当加上-g参数.
>gcc -g program.c -o grogram

完成编译后,使用以下命令用gdb载入chengxu
>gdb grogram

使用**r/run**命令启动程序

如果需要通过命令行给程序传递参数,可以在gdb命令行中使用以下命令完成
>run arg1,arg2,...argn

### 终止程序运行
使用**kill**命令

### 暂停程序运行
**CTRL-C**
### 程序重新运行
使用**c(continue)**重新运行程序．

当程序已经运行到某处，但是忘记在后面添加断点，可以使用CTRL-C暂停程序，设置断点，然后使用continue运行到下一个断点处．

## 断点(breakpoint)

### 设置断点
在gdb内,使用**breakopint**或简称**b**完成.
|命令|说明|
|-|-|
|b function|在函数function处设置断点.|
|b line|在当前文件底line行设置断点.|
|b file:line|在文件file中的底line行设置断点.|

**WARNING**
C++函数具有多态性，且可以重载，因此对C++函数设置断点需要指明函数从属的类和参数列表．

>break TestClass::testFunc(int)

### 临时断点
**tb(tbreak)**命令设置临时断点．临时断点仅仅暂停程序一次，然后就被移除．

### 查看断点
使用**info breakpoint**命令,或其简写**i b**

###　忽略断点
**ignore [breakpoint_number] [times]**
忽略断点times次．
###　清除断点
|命令|说明|
|-|-|
|clear breakpoint_location|使用断点位置|
|d(delete) breakpoint_number|使用断点标号|

### 开关断点
断点既可以被打开（enable)，也可以被关闭（disable)．
|命令|说明|
|-|-|
|enable breakpoint_number|打开标号为breakpoint_number的断点．|
|disable breakpoint_number|关闭标号为breakpoint_number的断点．|

## 观察点(watchpoint)
watchpoint可以看成是特殊的breakpoint.当watch的变量满足某种某种条件时暂停程序．

**WARNING**
watchpoint只能设置在**当前作用域内**的变量处，因此如果要设置non-global变量为观察点，需要先在作用域处设置断点．
### 设置观察点

|命令|说明|
|-|-|
|watch [variable]|当变量被写时，暂停程序．|
|rwatch [variable]|当变量被读时，暂停程序．|
|awatch [variable]|当变量被读或被写时，暂停程序．|

### 查看观察点
**info watchpoints**
由于watchpoint是特殊的breakpoint，使用**info breakpoints**也可以查看观察点．

### 删除观察点
**delete [watchpoiint_number]**

### 忽略断点
**ignore [watchpoint_number] [times]**
## 单步调试
单步调试使用**n(next)**和**s(step)**
 
**WARNING**
next命令＂不进入＂函数内，直接执行到下一条语句；
step命令＂进入＂函数内部执行．

## 一步执行完函数
**finish**命令可以一步执行完函数并返回到调用函数．

## 显示变量值
**print**，**disp(display)**，**printf**三个命令都可以显示变量值

* print
print只显示一次变量值

* printf
printf如同Ｃ语言中的同名函数，可以格式化的显示变量值，只是使用时不需要加括号．

* disp(display)
这个命令一直显示相应的变量值．
可以使用**info display**命令查看自己要求显示的变量列表
使用**undisplay**可以取消显示，后面跟变量的标号．

## 函数调用栈

###  查看函数调用栈

**bt(backtrace)**命令查看调用栈．
显示结果中，上面为栈顶．

###　改变当前frame
使用**frame frame_number**命令可以改变gdb使用者当前所处的frame

### 查看当前frame详细信息
**info locals**查看当前frame本地变量
**info args**查看本frame参数

以上两个命令配合**frame**命令使用．