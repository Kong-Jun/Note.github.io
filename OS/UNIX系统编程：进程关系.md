# UNIX系统编程：进程关系

UNIX是分时系统，同时运行着多个进程，进程之间相互联系，形成了进程组、会话等进程关系，这些进程关系会影响某些函数/系统调用和信号的行为。

## 进程的起始（the origin of processes）



所有的进程都有一源头，加电开机启动操作系统并登录（获取*login shell*）就是进程的起始。这里介绍传统的UNIX登录机制。

UNIX登录的过程一般分为两种：

- **终端登录（*terminal login*）**
- **网络登录（*network login*）**

终端登录就是在本地计算机中启动操作系统并取得login shell，比如没有安装桌面环境（KDE、GNOME等）的Linux系统，启动后就是命令行，要求输入用户名和密码，验证通过后就会取得login shell。

网络登录就是通过网络登录远程计算机取得login shell，比如腾讯云、阿里云的Linux服务器，登录后就是命令行黑框框，就好像是在本地登录的一样。

### 终端登录

1. 启动init进程，init进程可能会读取终端相关的配置文件，比如/dev/ttys。
2. 为每个终端fork出一个子进程，并使用exec函数执行getty例程。
3. getty例程打开终端设备（标准输出、标准输入、标准错误）、读取用户名、初始化环境变量，并使用exec函数执行login例程。
4. login例程接收密码，如果验证成功就修改工作目录到家目录、修改终端所有权、设置用户信息（UID、GID等）、初始化环境变量（SHELL、PATH等）后执行login shell。

5. 如果验证失败，终止进程，init进程再次执行步骤2。

通过以上步骤，用户就取得了login shell，并且login shell是init进程的子进程（exec只执行程序不改变进程ID)。login shell也会读取它的配置文件来初始化自身。



## 网络登录（待续）

网络登录与终端登录主要的区别在于





## 进程与进程组（process and process group）

进程组是多个相关的进程的集合，通过进程组ID（PGID）来标识。进程组中的进程一般是通过fork形成的。fork出来的子进程会继承父进程的PGID，因此成为了父进程进程组的一员。

每个进程组都可以有一个进程组组长（process group leader)，这个首领是PID与PGID相同的进程。

进程和进程组的概念可以类比现实生活中的家庭，家庭（进程组）中的成员（进程）都具有血缘关系（fork），一般会有一个家长（进程组首领）。

UNIX没有为进程组ID提供专门的数据类型，PGID的数据类型与PID相同，使用pid_t。

UNIX提供了两个获取PGID的函数：

> #include <unistd.h>
>
> pid_t getpgrp(void);
>
> pid_t getpgid(pid_t *pid*);
>
> ​										Return: process group ID of calling process, -1 on error.

若`getpid`参数为0则返回调用进程的PGID，`getpgrp()`相当于`getpgid(0)`。

除了获取PGID，UNIX还提供了加入新进程组和创建进程组的函数`setpgid`。

> #include <unistd.h>
> int setpgid(pid_t *pid*, pid_t *gpid*);
>
> ​										Return: 0 if OK, -1 on error.

当参数pid或gpid为0时，以调用进程的PID为参数。如果gpid存在就将调用进程加入到该进程组中，否则创建新进程组。

创建新进程组的情况一般是某进程让自己成为进程组首领，因为`fork`保证子进程的PID不等于任何已存在的PGID，所以这种情况下可以放心调用`setpgid(0, 0)`。

 使用`setpgid`时需要注意权限问题：

- `setpgid`只能将进程加入到本会话的进程组中。
- `setpgid`只能修改调用进程及其子进程的PGID。
- `setpgid`只能用于未执行exec函数的进程。

- `setpgid`不可以修改会话首领的PGID。

只要违反了以上规定，`setpgid`就会出错并设置errno。

因为`fork`函数要求子进程继承父进程的PGID，所以通过`fork`创建的进程都在最初的祖先的进程组中。当我们使用shell执行命令（外部命令），shell先fork出一个子进程，然后子进程exec相应的命令，但是通过`ps`命令可以发现在shell中执行的命令是进程组首领，和shell不在一个进程组中。这就是因为shell在exec命令时，先使用`setpgid`将执行命令的子进程设置为了进程组首领。

## 会话与控制终端（session and controlling terminal）图片。。。。。。。。。。。

进程组是多个进程的集合，会话是多个进程组的集合。会话中的进程组也有类似与进程组中的进程之间的关系。

比如在bash中执行以下命令

> proc1 | proc2 &
>
> proc3 | proc4 | proc5

当执行完第一行的命令后，shell创建了两个进程，这两个进程在同一个进程组中;执行完第二行命令后，shell创建了3个进程，处于另一个进程组中。

这两个进程组处于同一个会话中。



## 任务控制（job control）

## 通过shell执行程序（shell execution of programs）

## 孤儿进程和孤儿进程组（orphaned process and orphaned process group）

孤儿进程是指父进程在其运行过程中终止的进程。孤儿进程会被init进程收养，init进程会回收孤儿进程遗留的资源，确保孤儿进程不会僵尸进程。因为孤儿进程的这个特性，有时会故意让一个进程变成孤儿进程，避免僵尸进程的产生。

进程可以变成孤儿，进程组也可以变成孤儿。进程组变成孤儿的条件复杂一些，只要满足以下任意条件，进程组就可以被认为是孤儿进程组。

- 进程组中所有进程的父亲都在该进程组中。
- 进程组中所有进程的父亲都在另一个会话中。

孤儿进程组相比与一般的进程组，特殊之处在于当孤儿进程组中存在停止状态的进程时，内核会向孤儿进程组中所有进程先发送`SIGHUP`信号（默认行为是终止进程）再发送`SIGCONT`信号。如果不了解孤儿进程组，程序很可能会出现很怪异的现象

孤儿进程组还会影响`SIGTTOU`和`SIGTTIN`信号。当孤儿进程组中的后台进程试图通过读取终端输入或试图向终端写入数据时，不会产生这两个信号。



