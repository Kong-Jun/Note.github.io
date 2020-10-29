通常，GCC 会设置好系统头文件路径，如果不加任何选项，默认从系统头文件路径中查找头文件（`"`包裹的头文件优先从当前路径查找）。

比如在我的 Fedora 32 上 gcc 10 的系统头文件路径为：

```shell
 /usr/local/include
 /usr/include
```

有时头文件放置在非标准目录中，或者希望让 GCC 使用用户提供的系统头文件，这时就需要修改 GCC 头文件搜索路径。

GCC 中有以下几个和头文件搜索路径相关的选项：

- -I
- -iquote
- -isystem
- -idirafter
- -nostdinc: 禁止搜索标准库头文件
- -nostdinc++: 禁止搜索 C++ 标准库头文件
- -sysroot: 设置系统根目录，同时影响头文件搜索目录和库搜索路径
- -isysroot: 设置系统根目录，仅影响头文件搜索路径

GCC 搜索头文件的次序为：

1. 在当前目录搜索`"`包裹的头文件
2. 在`-iquote`选项制定的路径中`"`包裹的头文件
3. 查找`-I`指定的目录
4. 查找`-isystem`指定的目录
5. 查找系统头文件搜索目录
6. 查找`-dirafter`指定的目录

自己提供的头文件搜索目录安装命令行上的顺序进行搜索。

`-isystem`选项用于提供自己的头文件替换系统标准的头文件。比如，自己写了一个 stdio.h 并防止到当前目录，可以使用`-isystem .`选项使自己的  stdio.h 替换系统的 stdio.h。

使用`-sysroot`和`-isysroot`选项可以调整系统头文件搜索目录。这两个命令会给系统头文件搜索目录添加一个前缀。比如，系统头文件搜索路径是 /usr/include，使用`-sysroot .`或`-isysroot .`后，系统头文件搜索目录就变成了 ./usr/include。两个选择唯一的区别是`-sysroot`不仅会修改系统头文件搜索路径，还会修改库的搜索路径，而`-isysroot`仅影响头文件。

在不加选项时，GCC 优先搜索当前编译器版本的头文件目录，再搜索系统头文件目录。在我的 Fedora 32上， gcc 10 的搜索次序为：

```shell
GGC 准则：--param ggc-min-expand=100 --param ggc-min-heapsize=131072
忽略不存在的目录“/usr/lib/gcc/x86_64-redhat-linux/10/include-fixed”
忽略不存在的目录“/usr/lib/gcc/x86_64-redhat-linux/10/../../../../x86_64-redhat-linux/include”
#include "..." 搜索从这里开始：
#include <...> 搜索从这里开始：
 /usr/lib/gcc/x86_64-redhat-linux/10/../../../../include/c++/10
 /usr/lib/gcc/x86_64-redhat-linux/10/../../../../include/c++/10/x86_64-redhat-linux
 /usr/lib/gcc/x86_64-redhat-linux/10/../../../../include/c++/10/backward
 /usr/lib/gcc/x86_64-redhat-linux/10/include
 /usr/local/include
 /usr/include
搜索列表结束。
```



