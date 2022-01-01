- [ ] 怎样 clean 才能在删除不存在的文件时不报错

使用 find

- [ ] 怎样让 Makefile 可以“感知全局”？比如说我要依赖 lib/lib.a，不需要在当前目录中指定 lib/lib.a，Makefile 自动找到 lib/Makefile 中的 lib/lib.a 并构建它
- [ ] 怎样实现 CMake 中的 `add_subdirtory()`？现在是在 Makefile 中`make -C <directory> <target>`（切换到`<directory>`用其中的Makefile构建`<target>`),这种方法的问题是子目录的 Makefile 不会继承父目录中的变量。

- [ ] 使用编译器自动生成依赖

> GCC/Clang -MM -MT

# Makefile 问题

## 命令前的`@`，`-`有什么用？

## 如何使用通配符？

只能在 target 和 prerequisites 中直接使用通配符，不能在变量中使用通配符。变量中的通配符不会展开，请使用函数`wildcard()`

## 如何让 target 依赖于 requisites 但不使用时间戳更新？

## 如何替换变量的值？

变量替换

字符串替换函数

## 如何使用多目标规则？

independent target

group target

## GNU Make 如何处理一个目标多个规则的情况？

## 如何理解变量的两段展开？

## 如何定义自己的函数（宏）？

## 如何定义局限于特定规则的变量？

## 如何递归调用 Makefile？

## 如何将上层 Makefile 中的变量扩散到下层 Makefile？

## 如何让 Make 自动查找文件

`VPATH`变量

`vpath`指令

## 如何实现默认变量？

`?=`

## 如何方便的编译所有源文件都在一个目录的项目？

## 如何使用隐含规则？

## 为什么 Vim 无法正确处理 Makefile 的输出？

1. 语系环境变量为设置成中文，GNU Make 输出中文提示，Vim 的 errorformat 默认只支持英文。

在 errorformat 中添加对中文的支持：

```vim
" Support Chinese version of GNU Make
setlocal errorformat+=
            \%D%*\\a[%*\\d]:\ 进入目录“%f”,
            \%X%*\\a[%*\\d]:\ 离开目录“%f”,
            \%D%*\\a:\ 进入目录“%f”,
            \%X%*\\a:\ 离开目录“%f”
```

为了避免影响全局 errorformat，建议将这段代码添加进`.vim/after/{c,cpp,asm}vim`中，可以参考我的配置[nvim-config](https://github.com/kongjun18/nvim-config/blob/2e6ed4b304803a9373944203fb5f58520a4534c4/after/ftplugin/c.vim#L8-L13)。

2. Makefile 中使用了相对路径，而 Vim 没有设置 auchdir。

Vim 没有设置 auchdir（自动切换工作目录）， quickfix 匹配到到相对目录后解析错误。可以简单的在 Vim 中设置 auchdir 解决这个问题，但这指标不治本。建议修改 Makefile，使用绝对路径，可以通过`CURDIR`变量获得 Make 工作目录。

## 如何获取项目根目录

## 如何获取 Makefile 正在构建的目标



笔记：

语言：

- [ ] `@`，`-`
- [ ] 通配符
- [ ] order-only prerequisite
- [ ] 变量替换
- [ ] 多目标规则
- [ ] 一个目标多个规则的情况
- [ ] 两段展开
- [ ] 变量赋值
  - [ ] 自动变量 哪些可能会展开为列表，何时是文件，小心`$$@`[[Secondary Expansion](https://stackoverflow.com/questions/23214115/makefile-secondary-expansion)](https://stackoverflow.com/questions/23214115/makefile-secondary-expansion)

使用：

- [ ] 依赖树，源文件单独编译成 .o（不加链接选项），然后再将 .o 文件链接为kezhiixng文件

- [ ] 目标变量、模式变量            == CMake target-specific option

- [ ] 递归调用                               == add_sub_directory

  为什么递归的 Makefile 会在几个分离的 make 实例之间打破依赖树，并且常常会产生脆弱的构建。使用递归的 Makefile 毫无益处。好的选择是在项目的根目录放置一个 Makefile, 在那里进行调用。

- [ ] 查找目录`vpath`                   == include_dir

- [ ] `?=`实现优先使用用户提供的变量

- [ ] 编译只有单层目录的项目

- [ ] out-of-source 编译             

- [ ] 自动生成依赖

- [ ] 创建 GNU 风格的的目标及常用的变量（约定俗成的`PREFIX`,`DESTDIR`）

- [ ] 隐含规则

- [ ] 打印目录,`-k`与 Vim

在递归 Makefile 模式中，子目录 Makefile  中使用相对目录 ../include，如果 Vim 不设置 autodir，将无法使用 quickfix

可以在主目录中设置 SOURCE_DIR，然后 export 给子目录









## 待续

- `-`可以在命令失败后继续执行

- `@`禁止显示当前命令，只显示命令结果

- `include`和 C 语言的 `#include`一样，但是可以使用 sh 的通配符

- `$@`在多目标模式下代表触发的目标文件名

- 如果需要下一条命令依赖前一条命令，请将两条命令写到一行，并使用`;`分隔

  > cd subdir; pwd	

- 多行变量

```makefile
define run-yacc
yacc $(firstword $^)
mv y.tab.c $@
endef

foo.c : foo.y
    $(run-yacc)
```

- Make 默认使用环境变量`SHELL`指定的 shell（默认 UNIX sh） 执行命令。这里的环境变量和 shell 环境变量无关。
- 变量可以包含空白，因此要小心变量末尾的空格，尤其是之后有注释时

```Makefile
dir := /foo/bar    # directory to put the frobs in
```

`#`开始是注释，被认为是 EOL 标志，由于变量后面允许存在空格，因此`dir`被定义为`/foo/bar`加 4 个空格。

Makefile 允许变量使用空白主要是为了实现列表，如`src := main.c hello.c`

- `=`可以使用文件任意处定义的变量，容易出错，尽量使用`:=`（只能使用之前定义的变量）

- 变量替换：`$(var:a=b)`或静态模式（如`(SRC:%.c:%.o)`）

`$(var:a=b)`生成一个新变量，值是将`var`中以`a`结尾的部分替换为`b`

- `?=`: 如果变量未定义则定义，否则不做处理
- `+=`使用`:=`还是`=`取决于该变量上次使用的赋值运算符，一率使用`:=`赋值就可以避免`+=`使用`=`
- 命令行传入的变量有最高优先级，可以覆盖 Makefile 中定义的变量;Makefile 中定义的变量可以覆盖环境变量
- 嵌套调用 Makefile 时，默认上层目录的自定义变量不会传递到下层，环境变量会传递到下层。如果要传递，使用`export`，否则使用`unexport`。

```makefile
var := Top-level Makefile
export var
# 或 export var := Top-level Makefile
```

- 如果要覆盖命令行传入的变量，在变量定义前加上`override`
- 不建议通过 shell 环境变量传递值给 Makefile，当执行别的 Makefile 但没有清除环境变量时就会出错。
- Makefile 支持作用域限定在目标的变量（目标变量），可以实现类似 CMake 中`target_compile_option()`之类的功能。语法为：

```Makefile
<target> : <variable-assigment>
<target> : override <variable-assignment>
```

目标变量还可以再拓展，即模式变量。语法为：

```
<pattern> : <variable-assignment>
<pattern> : override <variable-assignment>
```

- Makefile 中的条件语句比较拉胯，只支持`ifeq (...) ... else ... endif`和`ifdef var ... else ... endif`及其`n`版本（`ifneq`、`ifndef`）。条件语句更像是 C++ 中的`constexpr if`，在运行前就执行，选择对应的语句，因此不要在条件语句中使用自动变量（他们在运行时创建）

```Makefile
var := hello
all:
ifeq ($(var), hello)
	@echo hello
else
	@echo world
endif
```



# Makefile 的变量

`MAKECMDGOALS`: Makefile 正在执行的目标名

`CURDIR`: 工作目录

# 自动变量

## 为什么修改源文件后 Makefile 没有发现更新？

可能是没有将头文件写入到规则中。

# 如何自动生成头文件依赖？

这里的“依赖”指头文件，考虑这种情况，我们在 Makefile 中 .o 文件仅依赖了 .c 文件，当 .c 文件中 include 的 .h 文件被更新时，Makefile 将不会发现更新，remake 时可能会错误地认为 .o 文件也不用更新。这种情况可以手写头文件依赖解决，但手写头文件依赖存在两个弊端：

1. 需要花费大量时间查找头文件依赖

2. 当 .c 文件添加/删除头文件时，必须及时更新

第一种情况比较容易解决，可以让编译器打印头文件依赖，再复制到 Makefile 中。智者千虑，必有一失，第二个问题很难手动解决，这个情况类似于 C++ 中的手动`new`/`delete`，C++ 使用 RAII 来自动完成内存分配/释放，我们这里也要想到某种自动化处理的办法来实现自动生成依赖。

解决这个问题最关键的两个技术就是：

- GCC/Clang 提供的自动生成头文件依赖功能
- GNU Make 的`include`指令

--------------------

## 如何生成 .d 文件

GCC/Clang 可以使用以下预处理选项生成 Makefile **规则**:

- `-M`: 打印源文件的所有依赖头文件（包括系统头文件)到标准输出
- `-MM`: 同上，但不打印系统头文件
- `-MD`和`-MMD`:  类似`-M`和`-MM`但生成后缀名为 .d 的依赖文件
- `-MF`: 执行生成的依赖文件的名字，和`-M`/`-MM`/`-MD`/`-MMD`一起使用
- `-MT`: 修改规则中的目标名

`-M`和`-MM`隐含了`-E`，只会预处理源文件（不会打印），而`-MD`和`-MMD`没有隐含`-E`，即不影响编译器的正常执行。通常使用`-MM`和`-MF`单独生成依赖文件，或使用`-MMD`在编译过程中生成依赖文件。

```shell
# 在编译过程中生成依赖文件
SHELL: gcc -c hello.c -MMD
SHELL: cat hello.d
hello.o: hello.c hello.h
# 修改目标名
SHELL: gcc -c hello.c -MMD -MT 'src/hello.c'
SHELL: cat hello.d
src/hello.c: hello.c hello.h
```

生成 .d 依赖文件的代码如下：

```Makefile
# 来自 GNU Make Manual
%.d: %.c
        @set -e; rm -f $@; \
         $(CC) -MM $< > $@.$$$$; \
         sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
         rm -f $@.$$$$
```

其中`set -e`指示 sh 当任何命令失败时退出。`$$$$`代表一个 4 位随机数。`sed`指令表示将 xxxx.d 添加到目标中。仍以上面的 hello.c 为例，生成 hello.d.xxxx ，内容为：

```Makefile
hello.o: hello.c hello.h
```

然后`sed`指令将 hello.d.xxxx 修改为：

```Makefile
hello.o hello.d: hello.c hello.h
```

再使用重定向将 hello.d.xxxx 写入到 hello.d 中，删除 hello.d.xxxx。

## 实现头文件依赖的自动生成

在第一次编译项目时，生成 .c 文件的依赖文件 .d（内含依赖关系），并使用 sed 修改 .d 的内容，使得依赖文件同时依赖于 .c 和  .h 文件，使用`include`指令将 .d 文件中的依赖关系引入到 Makefile 中。当我们在 .c 中修改`#include`指令，或修改头文件时，.d 文件会被更新，然后被 Makefile 包含。这样就确保了编写出来的 Makefile 任何时候都能感知头文件的更新，并且借助编译器的功能，大量减少了 Makefile 代码。

需要注意的是，我们在第一次编译项目时顺便生成依赖文件，因此第一次编译时`include`指令会失败，需要加上`-`前缀以正常运行。

这个代码仅仅是演示如何创建实现自动依赖生成所需的 .d 文件，更具体的使用方法放到后面的 Makefile 模板中介绍。

# 如何实现 out-of-source build

将构建生成的所有文件都放到一个二进制目录中可以保持项目的干净整洁，但很遗憾 Make 没有直接提供这项功能。很显然，从结果上看，out-of-source build 和最普通的直接在源文件目录中构建仅仅是中间文件存放的位置不同而已，因此只需要将源文件目录中的中间文件存放到二进制目录即可。

Makefile 使用依赖组织项目，因此应该利用依赖自动的将中间文件生成到二进制目录中。

```Makefile
# 来自 https://www.jianshu.com/p/d35a8291e2f4
incdirs := include include/src2
srcdirs := src src2

#指定源文件列表(由vpath处理路径问题)
srcs := vpath.c src2.c

#指定中间文件目录
objdir := obj
#指定目标文件列表
objlist := $(patsubst %.c, $(objdir)/%.o, $(srcs))

#指定gcc头文件路径
INCDIR := $(patsubst %, -I%, $(incdirs))

#为.c文件指定搜索目录
vpath %.c $(srcdirs)

#为.h文件指定搜索目录
vpath %.h $(incdirs)

.PHONY: all clean

objdir:
    @echo "create obj directory"
    -mkdir $(objdir)

$(objdir)/%.o: %.c
    gcc -MD $(INCDIR) -c $< -o $@

all: $(objdir) $(objlist)
    @echo compile done

clean:
    rm $(objdir)/*.o $(objdir)/*.d $(objdir) -rf
```

上面的代码块实现了最简单的 out-of-source build，所有 .o 文件都存放在 obj 目录中，后面的 Makefile 模板会实现更加复杂的 out-of-source build。

通过上面的代码可以看出，实现 out-of-source build 的关键在于：

- 变量替换/字符串替换：将.o 文件的目录修改到为二进制目录中
- `vpath`指令或`VPATH`变量：简化 requisite 

需要注意，使用`vpath`或`VPATH`存在错误处理不同目录下的同名文件的风险。

# Makefile 模板

这个模板来自 stackoverflow 的问答[building-c-program-out-of-source-tree-with-gnu-make](https://stackoverflow.com/questions/39015453/building-c-program-out-of-source-tree-with-gnu-make)。我觉得这个模板充分体现了编写 Makefile 需要遵守的原则，很值得学习，因此以这个模板总结我对 Makefile 的学习成果。

```Makefile
# Set project directory one level above the Makefile directory. $(CURDIR) is a GNU make variable containing the path to the current working directory
PROJDIR := $(realpath $(CURDIR))
SOURCEDIR := $(PROJDIR)/Sources
BUILDDIR := $(PROJDIR)/Build

# Name of the final executable
TARGET = hello-world

# Decide whether the commands will be shown or not
VERBOSE = TRUE

# Create the list of directories
# DIRS = Folder0 Folder1 Folder2
DIRS = Folder0
SOURCEDIRS = $(foreach dir, $(DIRS), $(addprefix $(SOURCEDIR)/, $(dir)))
TARGETDIRS = $(foreach dir, $(DIRS), $(addprefix $(BUILDDIR)/, $(dir)))

# Generate the GCC includes parameters by adding -I before each source folder
INCLUDES = $(foreach dir, $(SOURCEDIRS), $(addprefix -I, $(dir)))

# Add this list to VPATH, the place make will look for the source files
VPATH = $(SOURCEDIRS)

# Create a list of *.c sources in DIRS
SOURCES := $(foreach dir,$(SOURCEDIRS),$(wildcard $(dir)/*.c))

# Define objects for all sources
OBJS := $(subst $(SOURCEDIR),$(BUILDDIR),$(SOURCES:.c=.o))

# Define dependencies files for all objects
DEPS = $(OBJS:.o=.d)

# Name the compiler
CC = gcc

# OS specific part
ifeq ($(OS),Windows_NT)
	RM = del /F /Q
	RMDIR = -RMDIR /S /Q
	MKDIR = -mkdir
	ERRIGNORE = 2>NUL || true
	SEP=\\
else
	SHELL = /bin/sh
	RM = rm -rf
	RMDIR = rm -rf
	MKDIR = mkdir -p
	ERRIGNORE = 2>/dev/null
	SEP=/
endif

# Remove space after separator
PSEP = $(strip $(SEP))

# Hide or not the calls depending of VERBOSE
ifeq ($(VERBOSE),TRUE)
	HIDE =
else
	HIDE = @
endif

# Define the function that will generate each rule
define generateRules
$(1)/%.o: %.c
	@echo Building $$@
	$(HIDE)$(CC) -c $$(INCLUDES) -o $$(subst /,$$(PSEP),$$@) $$(subst /,$$(PSEP),$$<) -MMD
endef

# Indicate to make which targets are not files
.PHONY: all clean directories

all: directories $(TARGET)

$(TARGET): $(OBJS)
	$(HIDE)echo Linking $@
	$(HIDE)$(CC) $(OBJS) -o $(TARGET)

# Include dependencies
-include $(DEPS)

# Generate rules
$(foreach targetdir, $(TARGETDIRS), $(eval $(call generateRules, $(targetdir))))

directories:
	$(HIDE)$(MKDIR) $(subst /,$(PSEP),$(TARGETDIRS)) $(ERRIGNORE)

# Remove all objects, dependencies and executable files generated during the build
clean:
	$(HIDE)$(RMDIR) $(subst /,$(PSEP),$(TARGETDIRS)) $(ERRIGNORE)
	$(HIDE)$(RM) $(TARGET) $(ERRIGNORE)
	@echo Cleaning done !

```


# 总结

**实现**:

- 

**设计**：

- 遵守一定的规范

比如创建 GNU 风格的目标`all`、`clean`等，使用常见的变量名。

- 给用户提供选择的机会

比如模板中使用提供了变量`VERBOSE`决定是否打印命令

- Makefile 很难做到的事干脆让用户来做

Make 不擅长判断操作系统，不擅长自动查询目录，所以操作系统和源代码目录中的子目录由用户提供即可。

- 良好的提示信息

模板会在某项任务的执行过程中打印信息，如果用户不希望看到详细命令（`VERBOSE`为 0），仍可以看到比较精简的信息，对编译的过程有大致的了解。这一点有些类似于 CMake 的输出。

