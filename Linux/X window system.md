# X Window System 架构



# X Window System 启动流程



# X Server 配置文件

不同的 Linux 发行版使用的配置文件略有不同，但是总体结构差不多。我的 Fedora 32 中 X Server 不再使用单个的大的配置文件，而是使用配置目录，其中存放小的模块化的配置文件。

配置文件/目录如下：

- /etc/X11/xorg.config.d/

- /usr/share/X11/xorg.conf.d/

可以通过查看 /var/log/Xorg.0.log 前几行的内容查找自己系统的 Xorg 使用的配置文件。

除了配置文件 X Server 还需要一些其他资源，如字体、显卡驱动等。这些在资源存放在：

- /usr/lib64/xorg：Xorg 使用的一些动态链接库。
- /usr/share/X11: 字体、国际化等

通常情况下 X Server 的配置文件都是自动生成的，已经是最适合我们的系统的配置了，不需要进行调整。



# X11-Forwoarding

有时需要在服务器中运行 GUI 程序。这时可以使用 X window system 的 X11-forwarding 功能实现在本地计算机上显示、操作