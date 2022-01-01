# 时间片长度

# 数据结构

## 硬件相关的数据结构

硬件相关的数据结构主要是 LDT、GDT 和 IDT。这些数据结构相关的类型、宏定义在 include/linux/head.h 中。GDT 和 IDT 都定义在汇编文件（bootloader）中，在 include/linux/head.h 中将他们导出为 C 全局变量。

```c
typedef struct desc_struct {
	unsigned long a,b;
} desc_table[256];

extern unsigned long pg_dir[1024];
extern desc_table idt,gdt;
```

LDT 和 TSS 是局部的，每个进程都有一个和它关联的 LDT 和 TSS，因此 LDT 和 TSS 作为进程控制块的一部分。GDT 中存储 LDT 描述符和 TSS 描述符，指向进程控制块中的 LDT 和 TSS。

内核跑起来后 GDT 布局如下：

```
# 注意，这里的 LDT 和 TSS 分别指 LDT 描述符和 TSS 描述符。
# LDT 和 TSS 是特殊的段，不是 GDT 的元素
LDT125
TSS125
 .
 .
 .
LDT2
TSS1 --> 
LDT1--> 
TSS0
TEMP
DATA
CODE
NULL
```

Linux0.12 中 GDT 一共有 256 项，前 4 项不用于实现进程，每个进程都有对应的 TSS 和 LDT，因此系统最多支持 (256 - 4) / 2 = 126 个进程。Linux 0.12 进程数组总共有`NR_TASKS`(`64`)项，因此 Linux 0.12 实际上最多支持 64 个进程。

`GDT[0]`(`NULL`)、`GDT[1]`(`CODE`)、`GDT[2]`(`DATA`)和`GDT[3]`(`TEMP`) 在 bootloeader 中已经写入了对应的值。include/linux/head.h 中定义了几个宏描述了初始时 GDT 的布局：

```c
#define GDT_NUL 0
#define GDT_CODE 1
#define GDT_DATA 2
#define GDT_TMP 3

#define LDT_NUL 0
#define LDT_CODE 1
#define LDT_DATA 2
```

Linux 0.12 故意让 TSS 描述符和 LDT 描述符邻接着存放，这样可以方便地使用进程号（即进程控制块数组`task[]`中的下标）来获取对应 TSS 描述符和 LDT 描述符在 GDT 中的字节偏移。具体代码可见 include/linux/sched.h 中的`_LDT()`、`_TSS()`和`str()`宏。



## 进程控制块

进程控制块和内核堆栈聚合为一个联合体`task_union`：

```c
union task_union {
	struct task_struct task;
	char stack[PAGE_SIZE];
};
```

可以发现进程控制块`task_struct`和内核堆栈共用一页内存，进程控制块`task_struct`位于一页的低地址处，内核堆栈从页最高地址向下增长，堆栈最大大小为`PAGE_SIZE - sizeof(struct task_struct)`。进程控制块和内核堆栈间没有“分界线”，内核堆栈可以不断向下增长，并覆盖进程控制块，这时将产生难以预测的后果。

进程控制块中包含两类信息：

- 处理器状态信息：如 TSS(task state segment)、LDT 等
- 进程控制信息：如进程调度状态、进程组、当前工作目录等。

具体的结构体成员可以查看`struct task_struct`的定义。



# 创建进程

## 进程 0

第一个进程由内核硬编码创建，这个进程成为进程 0。在 Linux 0.12 中，进程 0 是内核自身，不参与进程调度，仅在系统无运行态进程时控制处理器。

第一个进程的控制块为`init_task`（一个联合体），被设置为`INIT_TASK`。

`INIT_TASK` 的 LDT:

```c
	{ \
		{0,0}, \
/* ldt */	{0x9f,0xc0fa00}, \
		{0x9f,0xc0f200}, \
	}, \
```

可见 LDT 中代码段和数据段基地址均为`0x00`指向内核所在的位置。

`INIT_TASK`的 TSS:

- esp0： `PAGE_SIZE+(long)&init_task` 
- ss0: `0x10`（GDT 第 3 项，全局数据段）
- cr3: `&pg_dir`
- 段寄存器: `0x17`（LDT 的第 3 项，局部数据段）
- ldt: `_LDT(0)`

内核跑起来后，自己将自己移入第一个进程（进程 0）`init_task`，该进程的控制块被初始化为`INIT_TASK`，其中设置好了 TSS 和 LDT，因此必须在切换到进程 0 前（进程切换会读取 TSS 和 LDT）设置好 GDT 中对应的描述符和寄存器 tr、ldtr。`main.c()`在调用`move_to_user_mode()`（代码段选择子设置为`0x0f`，数据段选择子设置为`0x17`）前在`sched_init()`设置好了相应描述符。如果不提前设置好描述符, 将访问到未定义的描述符，这种错误类似于 C 中访问空指针。

`move_to_user_mode()`通过`iret`指令，模拟中断返回的过程，将内核移入进程 0 中。移入之后，将进程 0 的段寄存器设置为`0x17`。

```c
#define move_to_user_mode() \
__asm__ ("movl %%esp,%%eax\n\t" \
	"pushl $0x17\n\t" \			# ss
	"pushl %%eax\n\t" \			# esp
	"pushfl\n\t" \				# eflags
	"pushl $0x0f\n\t" \			# cs
	"pushl $1f\n\t" \			# eip，压入的是 iret 后一条指令的地址，因此 iret 后执行之后的代码
	"iret\n" \
	"1:\tmovl $0x17,%%eax\n\t" \
	"mov %%ax,%%ds\n\t" \
	"mov %%ax,%%es\n\t" \
	"mov %%ax,%%fs\n\t" \
	"mov %%ax,%%gs" \
	:::"ax")	
```

进程 0 占据了线性地址空间的`0`至`64M`。事实上，Linux 0.12 只支持 64 个进程，其虚拟地址空间分别为`nr*64M`至`(nr+1)*64M`。

## `fork()`



# 销毁进程

1. 释放内存。通过`free_page_tables()`来释放进程占据的数据段和代码段。
2. 释放文件描述符。
3. 信号处理
   1. 如果当前进程推出导致所在进程组成为孤儿进程组，并且进程组内有停止的进程，向进程组内进程先发送 SIGHUP，然后发送 SIGCONT。
   2. 如果当前进程有孩子，通知 init 收养子进程。
   3. 通知父进程，子进程已结束
4. 设置为僵尸状态。

没有释放 PCB。

# 进程调度

## 调度的硬件机制

## 调度策略和算法



# 阻塞队列的实现

`__sleep_on()`

**问题**：

1. 为什么字符设备的阻塞队列只是一个指针？这是否意味着任意时刻最多只有一个进程阻塞在上面？
2. 为什么可以任务局部变量？













