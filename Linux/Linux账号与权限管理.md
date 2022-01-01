[toc]

## 用户配置文件

用户配置文件主要是 /etc/passwd 和 /etc/shadaw ，分配记录用户信息（用户名、 UID 、 GID 等）和用户密码信息（加密后的密码、最近一次修改密码的时间、密码失效时间等）。

/etc/passwd 和 /etc/shadaw 具有很高的保护级别，只有 root 才能修改，在 CentOS 8 中甚至只有 root 可以查看 /etc/shadaw 文件。

/etc/passwd 和 /etc/shadaw 配置文件在不同的 UNIX-like 系统上有着不同的格式，在不同的 Linux 发行版上也可能有细微的差别。下面说的格式是典型的 Linux 格式，CentOS 8 使用的就是这种格式

### /etc/passwd 文件格式

配置文件中的每一行的元素使用`:`分割，从左到右意义分别为：

1. 账号名称

2. 密码

   为了保证密码的隐蔽性，密码被加密后放在/etc/shadaw中，这个字段通常显示为X。

3. UID

4. GID

5. 注释

6. 家目录

7. shell

UID代表用户的“身份”，一般来说每个账号的UID都是独一无二的，但是这并不是强制要求，允许多个账号共享一个UID（不推荐）。

Linux中的账号分一般账号和系统账号。

