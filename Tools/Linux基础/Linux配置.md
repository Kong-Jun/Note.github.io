# Fedora 配置

[TOC]

使用 Fedora 已经快一年了，从 Fedora32 用到了 Fedora34，这篇文章记录安装配置过程中遇到的问题，部分问题在 Fedora 34 中已经不存在了，当仍然保留了解决方案。


## 更换软件源

修改Fedora软件源的方法参考[清华镜像站](https://mirrors.tuna.tsinghua.edu.cn/help/fedora/)，如果需要切换到别的镜像站，只需要将配置文件中清华镜像站的网网址修改为与切换的镜像站即可。

除了清华镜像站，还有[兰州大学镜像站]( mirror.lzu.edu.cn)等可用。兰大镜像站在学校里面特别快，而且免校园网流量。



## 中文输入法

输入法使用 fcitx5,该版本在 fcitx4 上做了大量优化，在大多数软件源较新的发行版生都有该包。安装 fcitx5，在`/etc/environment`中添加以下配置即可：

```
GTK_IM_MODULE=fcitx
QT_IM_MODULE=fcitx
XMODIFIERS=@im=fcitx
SDL_IM_MODULE=fcitx
```
fcitx5 支持云拼音和搜狗词库，可以在设置中添加，建议开启模糊音（如 c --> ch）。


## firefox安装flash插件（过时）

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

Yakuake 是 KDE 开发的下拉式终端，默认情况下宽度没有占满屏幕，在设置中将宽度设置为 100% 即可。



## 在中文环境下将默认目录修改成英文

为了避免语系导致的问题，在安装的时候直接选了中文，家目录里的几个目录默认是中文的，在命令行里很麻烦，如果直接修改这几个目录的名字，就无法通过文件管理程序访问这些目录了。可以通过手动设置 dolphin “文档“、”下载”等目录的位置来保证手动改名后文件管理程序可以正常访问到目录。



## Virtual Box

virtualbox 默认功能比较弱，如果需要共享文件夹、访问宿主系统设备等功能需要下载功能增强包。

virtualbox 的默认分辨率比较低，可以在全局设置中调节。

virtualbox 如果需要访问外围设备（比如U盘），不仅需要安装功能增强包，还必须将当前用户加入到 virtualbox 的用户组中。

```sh
cat /etc/group | grep vbox # 查找virtualbox所在的用户组
sudo usermod -a -G vboxusers username
```



## Typora 图片自动上传

在使用 Typora 编辑 markdown 文档时希望能将图片自动上传并将本地链接替换为 url，通过 PicGo 加 Gitee 可以满足这个需求。

使用 Gitee 作为图床，PicGo 作为管理图片上传的工具

PicGo 配置文件（`~/.picgo/config.json`）如下：

```json
{
  "picBed": {
    "current": "gitee",
    "uploader": "gitee",
    "gitee": {
      "branch": "master",
      "repo": "kongjun18/image", // Giteee 仓库
      "token": "×××××××××××",
      "customUrl": "",
      "path": ""
    }
  },
  "picgoPlugins": {
    "picgo-plugin-smms-user": true,
    "picgo-plugin-gitee-uploader": true,
    "picgo-plugin-github-plus": true
  },
  "picgo-plugin-gitee-uploader": {
    "lastSync": "2020-08-06 09:47:08"
  }
}

```





