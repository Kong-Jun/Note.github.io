# Fedora32一些问题的解决方案

[TOC]

电脑换成了thinkpad x1c 2019，完美支持Fedora 。fn键、背光键盘和其他所有设备(除了红外线摄像头)都可以正常运行，续航似乎没有下降，使用tlp电源管理程序后可以获得充电阈值功能。

切换到Fedora32 KDE版上遇到了一些问题，记录下来方便下次查询。 



## 更换软件源

修改Fedora软件源的方法参考[清华镜像站](https://mirrors.tuna.tsinghua.edu.cn/help/fedora/)，如果需要切换到别的镜像站，只需要将配置文件中清华镜像站的网网址修改为与切换的镜像站即可。

除了清华镜像站，还有[兰州大学镜像站]( mirror.lzu.edu.cn)等可用。兰大镜像站在学校里面特别快，而且免校园网流量。



## 中文输入法

在Fedora32上尝试安装了ibus-rime，安装后无法使用，切换到中文后无法输入字符。而且再次登录后，登录界面弹出了一个英文虚拟键盘，登陆界面无法输入回车，导致无法进入系统。

建议直接使用fcitx-pinyin。fcitx-pinyin在各种系统上都可以通过包管理器直接安装。在Fedora32上遇到了无法在kde应用中使用输入法的情况。只需要向~/.pam_environ中文ment添加一下三行内容即可修复：

```
GTK_IM_MODULE=fcitx流量
QT_IM_MODULE=fcitxhttps://support.mozilla.org/zh-CN/kb/flash-blocklists
XMODIFIERS=@im=fcitx流量
```



## firefox安装flash插件

flash很快就要停止支持了，各大浏览器都默认禁止用了flash，但是仍然有大量的视频网站使用flash，为了看乘风破浪的姐姐们，不得不安装flash。

没有可用的flash时，视频网址会弹出安装flash提示框，点开选择相应的发行版(deb和rpm)安装，如果没有对应发行版的安装包就下载tar.gz包，解压后按照README的说明安装。64位系统的firefox插件目录在/usr/lib64/mozilla/plugins。

安装后要重启浏览器或重启计算机。

安装后可能还是无法浏览视频，这可能是一下集中原因导致的：

1. 动态链接库libflashplayer.so的位置不对：存放在/usr/lib64/mozilla/plugins中。
2. 动态链接库libflashplayer.so的权限不对：修改权限为755。

以上两个问题主要是用tar.gz包安装会遇到。如果没有以上问题，还是无法浏览视频，那就是firefox设置的问题。

在”附加组件“-->”插件“，中应该能看到Shockwave Flash，在”首选项“中可以看到这个插件对应的文件是libflashplayer.so，这个就是flash插件。点开右边的”三个点“，选择”需要时询问“或“始终激活”（这个选项在Firefox69后被取消了），当网站请求flash插件时，就会出现运行flash的请求，同意后就可以使用flash浏览视频了。

芒果TV需要开启flash的“始终激活”选项，如果开启“需要时激活”选项，芒果TV不会在需要时请求，而是直接提示flash插件异常。这时可以在地址栏中看到一个淡灰色的图标，鼠标移上去显示“管理插件使用”，点开选择“允许”，刷新网页即可观看。

![](https://gitee.com/kongjun18/image/raw/master/Screenshot_20200627_130647.png)

如果成功安装、启用flash后还是无法使用，可以尝试用` sudo setsebool -P unconfined_mozilla_plugin_transition 0`关闭SELinux对Fifefox插件的控制。



## 安装解码器

很多发行版都没有默认安装视频解码器，导致在线看视频时提示“没有可用的视频格式”。在Fedora32上安装视频解码器的命令为：

```shell
sudo dnf install https://download1.rpmfusion.org/free/fedora/rpmfusion-free-release-$(rpm -E %fedora).noarch.rpm https://download1.rpmfusion.org/nonfree/fedora/rpmfusion-nonfree-release-$(rpm -E %fedora).noarch.rpm

sudo dnf install  gstreamer1-libav

```



## 编译安装Vim

编译安装vim可能遇到两个问题:1. 无法开启python3支持 2. 无法找到终端函数库

为了避免第一个问题，尽量在刚安装好系统后安装python3,确保没有多个版本共存，有时还需安装名字带devel的python3包，然后直接编译。如果没有找到python3位置，在configure时制定python3库的位置（不要使用--with-command)。如果编译时提示python3库缺少文件，可以卸载python3包重装或者制定别的版本的python3库的位置。

第二个问题是没有安装终端函数库导致的，只需要任意安装ncurses、curses、tinfo等包即可。在Fedora32上遇到了安装了终端函数库，但即使是使用`--with-tlib`制定了库位置也无法正确链接的情况，stackoverflow给出了解决办法：在运行configure之前执行`CFLAGS+=-fPIC`。https://jingyan.baidu.com/article/ff42efa9c95be8c19e220290.html

如果编译出来的Vim有不能接受的毛病，比如在我的Fedora上，安装ycm后使用vim-floterm打开悬浮终端时ycm会报错，可以安装neovim。安装neovim并开启python3支持的命令如下：

```
sudo dnf copr enable agriffis/neovim-nightly
sudo yum install neovim python3-neovim
```



## 关闭蓝牙开机自启

Fedora32开机后会自动打开蓝牙，为了关闭开机自启，只需要修改蓝牙的配置文件/etc/bluetooth/main.conf，将其中的`AutoEnable=ture`修改为`AutoEnable=false`



## yakuake无法正常使用

yakuake在Fedora上只有正常宽度的一半，没有找到调节正常的方法。使用guake替代yakuake。



## 在中文环境下将默认目录修改成英文

为了避免语系导致的问题，在安装的时候直接选了中文，家目录里的几个目录默认是中文的，在命令行里很麻烦，如果直接修改这几个目录的名字，就无法通过文件管理程序访问这些目录了。可以通过手动设置系统“文档“、”下载”等目录的位置来保证手动改名后文件管理程序可以正常访问到目录。

系统默认的用户目录配置文件在~/.config/user-dirs.dirs，手动修改文件名后再修改该配置文件中的文件名就行了。

修改后还是存在问题，dolphin但是可以通过家目录访问，但无法通过中文名访问到正确的目录。其他程序打开的文件管理器可以正常工作。暂时还没发现好的解决方法，不过这个小问题对我没有影响。



## Virtual Box



