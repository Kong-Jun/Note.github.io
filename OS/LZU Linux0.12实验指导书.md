# 第零章

本章介绍实验操作系统的环境搭建和相关开发工具。

## 环境搭建

本书的程序最终可以汇编、链接为可执行程序，并运行于 x86 虚拟机 bochs 中。

本书所有实验都在 Ubuntu 18.04 中完成，安装 bochs 的 debugger 版本的的命令如下：

```bash
sudo apt-get install build-essential libx11-dev xorg-dev libgtk2.0-dev
wget https://sourceforge.net/projects/bochs/files/bochs/2.6.8/bochs-2.6.8.tar.gz
tar -zxv -f bochs-2.6.8.tar.gz
cd bochs-2.6.8
./configure --enable-debuger --enable-disam --enable-debugger-gui
make -j$(nproc)
sudo make install
```

**注意**：

- Windows 用户如果要做实验，可以在虚拟机（virtualbox，vmare等）中安装 Ubuntu 18.04。
- bochs 的源代码托管在 sourceforge.net，有时下载会卡顿，可以过段时间再尝试下载。
- 实验使用 Ubuntu 18.04，但是其他 GNU/Linux 发行版（如 Fedora 32）也可以正常搭建环境。
- 不建议通过包管理器安装编译好的版本，可能出现某些文件路径不匹配、加载可执行文件后死循环等问题。

## 工具的使用

本书的实验使用 GNU 工具链编译链接程序，使用 dd 命令制作启动盘，使用 shell 构建项目。

构建项目的命令如下：

```bash
cd test3
sh run_test3.sh
```

清理项目生成文件的命令如下：

```shell
cd test3
sh clear_test3.sh
```

运行可执行文件的命令如下：

```sh
sh run_test3.sh	# 在项目根目录中（~/.../your-path-to-project）
```

可以组合以上命令，在`test3`目录中构建并运行项目可执行文件：

```shell
sh run_test3.sh && cd .. && sh ./run.sh && cd test3
```

使用命令别名简化流程：

```shell
alias build='sh run_test3.sh && cd .. && sh ./run.sh && cd test3'
```

设置好命令别名后，只要在`test3`目录中键入`build`即可构建并运行项目，其他实验的构建运行类似。

本书实验用到的所有命令都可以在 man pages 中查看使用方法，以查看命令 dd 的使用方法为例：

```shell
man dd
```

使用 bochs 调试 OS 的方法详见*实验参考书*。

# 

# 第一章：从实模式到保护模式

本章实验将完成 bootloader，简单的操作系统内核（仅能打印字符），并将处理器从实模式切换到保护模式。



## 实验目的

通过本章实验，可以了解到：

- 计算机硬件
  - 实模式和保护模式
  - BIOS
  - 外设的初始化
  - 保护模式下的分段内存模型
- 操作系统
  - bootloader、操作系统的编译、链接
  - 启动盘的制作
  - GDT 的设置
  - 简单的驱动程序



## 项目组成

本实验的项目文件如下：

```shell
.
├── bochsrc.bxrc
├── c.img
├── run.sh
└── test1
    ├── bootsect.s
    ├── clear_test1.sh
    ├── kernel
    │   ├── driver
    │   │   └── display.s
    │   └── head.s
    └── run_test1.sh
```

文件的说明如下：

**bootloader 部分**：

- test1/bootsect.s: bootloader 的源代码，这个文件编译后会成为硬盘的第一个扇区。

**内核部分**：

- kernel/driver/display.s：字符设备驱动，可以打印字符信息。
- kernel/head.s：内核的入口函数，完成内核的初始化并进入死循环。

**启动盘**：

- c.img: 最终项目生成的文件，这个文件作为硬盘被 bochs 识别并加载其中的 bootloader 和操作系统。

本实验主要修改其中的 test1/bootsect.s 和 kernel/head.s 文件。



## 实验内容

1. 在 bootsect.s 中完成以下内容：清空显示器屏幕上的所有内容，再往屏幕上打印一架红色的“字符飞机”。飞机造型如下：

```
                                    *  
                                  *****
                                    *  
                                   * *  
```

**提示**：

- 调用 BIOS 0x10 号中断处理程序清屏
- 字符飞机的打印可以使用循环完成



--------

2. 修改 bootsect.s，不使用任何符号，使用符号表示的数替换符号。

**提示**：编译出来的重定位文件 bootsect.o 中含有符号的信息，可以用 nm 命令查看。



------

3. 修改 bootsect.s，实现在不修改寄存器 cs 的值的情况下，让 bootloader 正常运行。

**提示**：

- 伪指令`.org`可以修改修改文件内指令的偏移地址。
- 链接器 ld 的 -Ttext 选项也可以设置偏移地址。
- 命令 dd 可以设置拷贝的起始块号和块大小。

-------------

4. 内核的文件包括 head.s 和驱动 display.s，bootloader 加载内核到线性地址 0x7e00，并跳转到 0x7e00 执行，怎样确保内核入口 `kernel_start`在 0x7e00？

**提示**：请查看构建脚本 run_test1.sh，注意链接过程和`kernel_start`在 head.s 中的位置。



---------------------

5. 修改 head.s，将线性地址空间 0x7e00 ~ 0x7ff 中的可执行文件 kernel.bin 拷贝到线性地址空间 0x00 ~ 0x1ff 中。head.s 中的指令同时存在于 0x000 ~ 0x1ff 和 0x7e00 ~ 0x7ff 中，在 0x000 ~ 0x1ff 中重新初始化 GDT，再跳转到 0x7e00 ~ 0x7ff 中的部分继续运行。

**提示**：

- 初始化 GDT 后，需要重新加载段寄存器。
- 注意保持偏移地址不变。



------------------

6. 修改整个项目，将 GDT 中的第 2 项（内核数据段）和第 3 项（显存）合二为一，并确保可执行文件正常运行。



------------

7. 修改整个项目，将内核的栈段和内核代码/数据段分开，将栈的线性地址空间初始化为 0x9000 ~ 0x91ff。



----------------

8. 在 7 的基础上修改 GDT，使内核代码段的段描述符的段边界设置为最小值。

**提示**：

- 可执行文件 kernel.bin 的大小可以通过`ls -l kernel.bin`查看。
- 分配栈空间的汇编指令为`.fill 50 4 0`，可以从中计算出栈空间大小。



