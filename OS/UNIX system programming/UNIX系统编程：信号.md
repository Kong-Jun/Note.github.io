# NOTE

- SIGHUP Controlling terminal session daemon

- SIGNAL TERMINAL IO
- `fork(2)`出的子进程继承父进程掩码，`execve(2)`不改变信号掩码
- `fork(2)`出的子进程 pending 为空，`execve(2)`不改变 pending
- 信号默认使用进程栈，但也可以通过`signalstack(2)`来设置单独的栈
- 每个线程都有自己单独的信号掩码，通过`pthread_sigmask(3)`设置
- 信号可以 direct 到线程（`pthread_kill(3)`，`tgkill(2)`），也可以到进程。硬件异常到线程吗？
- 线程既可以接受 thread-directed 信号，也可以接受 process-directed 信号。当进程接受到信号时，信号随机分配给线程。
- `siglongjmp(3)`是否恢复 signal mask，这取决于`sigsetjmp(3)`。
- 多个 standard signal 处于 pending 时，投递次序不确定
- standard signal 没有排队语义，当信号被 masked 时，同一信号的多个实例仅保留（投递）第一个。

# 信号概述

## 什么是信号

##　信号的种类

## 信号的行为

当前工作目录

/proc/sys/kernel/core_pattern

以下情况不生成 core

- 设置了SUID
- SGID
- 没有写权限
- 文件太大

# signal()

signal()

SIG_DFL SIG_IGN SIG_



`exec()`行为是捕获的信号修改为 SIG_DFL。`exec()`后，进程不再拥有之前 signal handler 的代码，因此修改为 SIG_DFL 较为合理。

`fork()`出来的进程继承父进的程的信号布局。`fork()`后，子进程继承父进程的内存空间，因此这个行为比较合理。凤飞飞单点DDDdddｄdｄdd

## 从不可靠信号到可靠信号

## 异步信号安全

## 信号的初始布局

## 信号的发送

## sigsuspend

1. 实现类似于条件变量的功能
2. 等待某特定的信号发生（只能被特定信号唤醒的pause）

## setjmp的应用

## abort

## system

- 注意可移植性，避免`sigaction()`和`signal()`的交互，这种行为通常取决于实现。小心`longjmp()`的不可移植性，小心`signal()`的不可移植性。
- 避免异步信号不安全函数被信号打断
- 谨慎地使用异步信号安全函数
- 谨慎地暂存恢复 errno