# 怎样用 CMake 实现一下功能

- [ ] 怎样用 CMake 运行构建出来的可执行文件

- [ ] 怎样实现类似`make clean`的功能

- [x] 函数参数是什么类型，拷贝还是引用？

  > 函数参数都是按值传递的

- [ ] 为不同的目标、文件设置不同的语言特性、编译选项

- [ ] 生成器表达式

# CMake简介

CMake是一个通过配置文件自动生成原生构建工具所用的文件（比如Makefile）的工具，并且可以自动生产多种格式的文件（例如VS、XCode的项目构建文件），实现跨平台跨构建工具。

![](https://gitee.com/kongjun18/image/raw/master/Screenshot_20200806_094650.png)

# CMake的执行过程



子目录中的`CMakeList.txt`继承父目录中定义的变量(副本),在子目录中`unset`继承来的变量不会影响父目录中的变量。

教程中说 `set(... CACHE ...)`会清除同名普通变量，但是在`cmake 3.17.4`中不会清除普通变量。



# CMake 版本的兼容性





# 作用域、变量类型、缓冲变量和普通变量、定义和修改变量





**命名习惯**：仅在单一作用域中使用的变量使用小写字母命令，在多个作用域中使用的变量使用大写字母命名



# 变量引用/



# 列表

- [ ] 枚举：STRINGS PROPERTY

- [ ] 空列表



----------



普通变量相当于许多语言中的局部变量，作用域为所在文件或函数;缓冲变量相当于全局变量，在所有`CMakeLists.txt`中都可以使用。



CMake 引用变量时，如果可以找到普通变量，就使用该变量，如果找不到普通变量就是用缓冲变量，如果找不到缓冲变量，就将该变量当作空字符串。可以在`CMakeLists.txt`中定义缓冲变量，也可以在命令行中传递缓冲变量`cmake -D var=value`，如果缓冲变量`var`存在，则将`value`赋给`var`;若`var`不存在，则创建`var`，并将`value`赋给它。



## 缓存变量

使用`set(var value type comment [FORCE])`命令设置缓存变量使用`unset(var CACHE)`命令取消缓存变量。

使用`set()`设置已经存在的缓存变量不会修改已存在的缓冲变量，如果要修改缓冲变量只能通过在命令行传递参数完成。通过命令行传递的缓冲变量优先级高于文件中定义的缓存变量。

如果要设置多个缓冲变量，可以将多个缓存变量定义在`xx.cmake`文件中，运行 CMake 时通过参数`-C xx.cmake`来读入变量。

如果一定要在文件中使用`set()`覆盖缓存变量，可以在`set()`命令的最后加上`FORCE`，但好的 CMake 代码不应该出现`FORCE`。

缓存变量类型除了`STRING`还有`BOOL`、`PATH`、`FILEPATH`等，变量实际的类型只有`STRING`，不同的命令将变量解释为不同的类型，这里提到的类型`BOOL`等是对于 CMake-GUI 而言的。缓存变量还有一个特殊的类型`INTERNAL`，表示这个变量仅在命令行 CMake 可见，并且隐含`FORCE`。

`option(var comment ON/OFF)`命令可以设置布尔类型的缓存变量，值为`OFF`或`ON`。

**最佳实践**：因为缓存变量是全局的，所以缓存变量，尤其是`option()`命令设置的选项，应该前缀项目名，以避免其与子目录中定义的缓存变量发生命名冲突。

## 环境变量

这里说的环境变量不会影响 shell，只影响 CMake。环境变量会被 CMake 的子进程继承，并且会存在于 CMake 的整个生命期。缓存变量仅在配置阶段存在，而环境变量在构建阶段仍然存在。

CMake 提供了在 CMake 文件中设置环境变量的功能，使用`set(ENV{var})`命令设置环境变量，使用`unset(ENV{var})`变量取消环境变量，使用`$ENV{var}`访问环境变量。比如：

```cmake
cmake_minimum_required(VERSION 3.5)
project(env NONE)
message("USER: $ENV{USER}")
set(ENV{USER} "nobody")
message("USER: $ENV{USER}")
unset(ENV{USER})
message("USER: $ENV{USER}")
---------------------------------------------------
|| USER: kongjun
|| USER: nobody
|| USER: 

```

CMake 可以读取 shell 设置的环境变量，但不能改变它，设置同名环境变量仅影响 CMake。

**陷阱**：CMake 不会追踪已使用的环境变量，如果通过 shell 修改了正在使用的环境变量，CMake 不会发现它被修改了，直到下一次运行 CMake 生成 Makefile。所以如果通过 shell 修改了环境变量，应当重新运行 CMake 生成 Makefile。



# CMake 代码组织与 listfile

**注意**：

1. 常用变量（`CMAKE_SOURCE_DIR`等）都是**绝对路径**
2. `CMAKE_CURRENT_LIST_FILE`、`CMAKE_CURRENT_LIST_DIR`等和当前 listfile 相关的变量都是指**当前执行**的 listfile，而不是函数定义所在的 listfile
3. 不要直接通过`${CMAKE_BINARY_DIR}/executable`来描述最终生成的可执行文件`executable`，因为在许多情况下可执行文件并不直接在`${CMAKE_BINARY_DIR}`中，可能在`${CMAKE_BINARY_DIR}/Debug`中



# 跨平台



`cmake -E`和`cmake -P`

CMake 是跨平台的工具，为常用的系统命令提供了跨平台的 CMake 命令，比如



# 控制流

## 分支结构

```cmake
if(condition)
elseif(condition)
else()
endif
```

其中，`condition`是一个变量或字符串。

CMP0054 

## 循环结构

1. `foreach(var list)`

```cmake
foreach(var list)
	# code
endforeach
```

2. `foreach(var RANGE [start] [end] [step])`

range 的范围为$[start, end]$,步长为`step`。步长可以忽略，默认为1。CMake 的 range 是闭区间，而不是左闭右开，这里要注意。

3. `while(condition)`
4. `break()`和`continue()`



## 函数

CMake 中的函数比较丑陋，语法上没有返回值，参数的语义也比较混乱。

函数的定义:

```cmake
function(<function_name> <args>...)
	# body
	# return()
endfunction()

```

CMake 中的函数参数是按值传递的（值语义）。

CMake 提供了多种访问/处理参数的办法。预定义了`ARGC`表示参数个数，`ARGVN`表示第 N 个参数（从 0 开始）。这是直接匹配的方式，是最简单清楚的参数传递、解析方式。比如：

```cmake
# 打印变量 msg 的内容
function(print msg)
	message("${msg}")
endfunction()
```

现代 CMake 还提供了`cmake_parse_arguments(<prefix> <optional>  <one_argument_keywords> <multiple_argument_keywords> <args>)`命令来解析参数，使得参数传递更加灵活（也更加混乱了）

```cmake
function(MY_PARSE)

    set(options ADULT)
    set(oneValueArgs NAME AGE)
    set(multiValueArgs SCORE)

    cmake_parse_arguments(STUDENT "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

    # 通过 prefix_参数名: 例如 STUDENT_ADULT
    message("isadult  = ${STUDENT_ADULT}")
    message("name  = ${STUDENT_NAME}")
    message("age  = ${STUDENT_AGE}")
    message("score  = ${STUDENT_SCORE}")

endfunction()

MY_PARSE(ADULT NAME zhangsan AGE 20 SCORE 100 200 300 400 500)

--------------------------------------------------------------------------------------------------
isadult  = TRUE
name  = zhangsan
age  = 20
score  = 100;200;300;400;500

```

## 待续（`cmake_parse_arguments()`）



CMake 中的函数可以使用`return()`命令终止函数，但是不能带出返回值。如果需要带出返回值，只能通过`set(var value PARENT_SCOPE)`命令设置父作用域的变量来模拟返回值，这个变量的名字通常由调用者传入函数中。例子如下：

```cmake
cmake_minimum_required(VERSION 3.5)
project(fun NONE)

function(print_if predicate msg ret)
	if (${predicate})
		message("${msg}")
		set(${ret} YES PARENT_SCOPE)
	else()
		set(${ret} NO PARENT_SCOPE)
	endif()
endfunction()

print`target_include_directories()`

`target_link_libraries()`_if(YES "Hello world" ret)
message("return value: ${ret}")
	
----------------------------------------
|| [cmake -S. -B _builds && cmake --build _builds]
|| -- Configuring done
|| -- Generating done
|| -- Build files have been written to: /home/kongjun/Documents/cpp_practice/cmake/function/_builds
|| Hello world
|| return value: YES

```



# 构建可执行文件



`target_include_directories()`

`target_link_libraries()`

`target_link_directories()`

`target_sources()`

# 构建库



# 控制编译选项、语言特性、传递宏



`set_target_property()`

`target_compile_options()`

`target_compile_features()`

`target_compile_definition()`



# 检测环境（操作系统、编译器、处理器）





# 测试



## 一般步骤

**注册测试**



**运行测试**

## GTest

**查找/安装 GTest**

`FindGTest`

`FetchContent`



**运行 GTest**

`set_test_properties()`

`GoogleTest`

