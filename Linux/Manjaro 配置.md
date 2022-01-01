# BIOS 设置

- Security -> Secure Boot - Set to "Disabled"：关闭安全启动。
- Security -> Virtualization -> Kernel DMA Protection - Set to "Disabled"：禁用 DMA　保护才能开启　Thunderbolt BIOS Assist Mode。
- Config -> Thunderbolt BIOS Assist Mode - Set to "Enabled"：节省电量。
- Config -> Power -> Sleep State - Set to "Linux"：开启 Linux 休眠模式。



#　安装更新固件

使用 LVFS 服务自动安装固件：

```shell
$ fwupdmgr get-devices # 获取设备列表
$ fwupdmgr refresh　＃从 LVFS 下载最新元数据
$ fwupdmgr get-updates # 获取更新
$ fwupdmgr update # 更新固件
```



# 声卡无法使用

/etc/pulse/default.pa 添加一下两行：

```
load-module module-alsa-sink device=hw:0,0 channels=4
load-module module-alsa-source device=hw:0,6 channels=4
```



# 更换镜像站

使用最快的镜像站。

```shell
sudo pacman-mirrors --fasttrack
```



# 启用 TRIM

启用 trim 定时器延长 SSD 寿命

```shell
sudo systemctl enable fstrim.timer
```



# 中文输入法

安装 fcitx rime 输入法：

```shell
yay -S fcitx5-im # 安装 fcitx5 框架
yay -S fcitx5-rime # 安装 rime 输入法引擎
yay -S rime-cloverpinyin # 安装 cloverpinyin 输入方案
yay -S fcitx5-pinyin-zhwiki-rime # 安装中文 wiki 词库
```

配置环境变量，将一下设置写入　~/.pam_environment。

```
GTK_IM_MODULE DEFAULT=fcitx
QT_IM_MODULE  DEFAULT=fcitx
XMODIFIERS    DEFAULT=\@im=fcitx
SDL_IM_MODULE DEFAULT=fcitx
```

创建　rime-cloverpinyin 输入方案，将一下设置写入　~/.local/share/fcitx5/rime/default.custom.yaml。

```
patch:
  "menu/page_size": 5
  schema_list:
    - schema: clover
```

下载 rime 词库到 ~/.local/share/fcitx5/rime。

安装 nord-light 输入法皮肤：

```shell
git clone https://github.com/tonyfettes/fcitx5-nord.git
mkdir -p ~/.local/share/fcitx5/themes/
cd fcitx5-nord
cp -r Nord-Dark/ Nord-Light/ ~/.local/share/fcitx5/themes/
```



# dropbox 无法登录

yay 安装的 dropbox 无法打开登录页面，可以从 dropbox 官方源码编译安装。



# electron-ssr 开机自动启动

electron-ssr 的 appimage 不要直接放到 /usr/local/bin 中，应该直接执行，electrion-ssr 会自动安装。

electron-ssr 中设置自动启动失败后，打开 KDE 系统设置，点开 electron-ssr 的设置，将命令改为 ~/Applications 目录下的 electron-ssr 配置文件。