**Lua 语言基础**

[TOC]

# 变量类型



`type()`

`math.type()`

# 逻辑类型

比较运算符的结果不是真值，而是运算对象，运算运算对象在被转换为真值。Lua 中经常使用布尔类型和逻辑运算符当作控制流。

# 数字

# 字符串

Lua 中的字符串是不可变（*immutable*）的。字符串是一个字节序列，可以在其中存储数据（类似与 C 语言），默认假设字符是但单字节的，但是提供了 UTF 8 编码等操作。

Lua 中的字符串字面量既可以使用`"`包围，也可以使用 `'`包围，两者是完全等效的。字符串支持 C 风格的转义字符。

Lua 支持*长字符串*（*long string*），长字符串使用匹配的`[[`和`]]`包裹起来，其中所有的转义字符和回车会被保留，如果第一个字符是回车，会清除掉这个换回车。长字符串不是一种新类型，只是一种在特定场合（大量文本）书写更方便的形式。不建议在长字符串中存储数据，如果需要存储在字符串中，建议使用括号包围的字符串写法。

```lua
- 以下两种写法的输出是相同的
print("on one line\non next line")
-------------------------------------
print([[
on one line
on next line]])
```

字符串连接操作符为`..`。

字符串可以和数字可以相互转换。当数字操作走用于字符串时，字符串转换为数字;当字符串操作作用于数字时，数字转换为字符串。字符串和数字之间的自动转换是“二等公民”（*second-class status*），Lua 没有完全集成字符串到整数之间的转换，字符串转换为数字时都被转换为浮点数。所有数字都可以自动转换为字符串。

字符串和数字的相互转换不发生在条件比较式中，如果比较数字和字符串会报错。试想，如果在比较中字符串和数字可以相互转换，数字有数字比较的规则，字符串有字符串比较的规则，这就会发生歧义。

字符串连接的符号是`..`，恰好和小数点`.`符号冲突，所以如果要将两个数字自动转换为字符串并连接在一起需要在两个数字之间加空格。

```lua
> 10..24
stdin:1: malformed number near '10..24'
> 10 .. 24
1024

```

字符串标准库和 UTF 8



# 表

**形式**

**相等性**

```lua
> monday = "sunday"
> sunday = "monday"
> t = {sunday = "monday", [sunday] = monday}
> print(t.sunday, t[sunday], t[t.sunday])
monday	sunday	sunday

```

table 中的 *key* 是字符串，不是变量，`[key]`形式是变量。



```lua
> a = {}
> a.a = a
> a
table: 0x558ed4ebacc0
> a.a
table: 0x558ed4ebacc0
> a.a.a.a
table: 0x558ed4ebacc0
> a.a = 3
> a.a
3
> a
table: 0x558ed4ebacc0

```



**序列和线性表及其长度运算符**

**遍历**

safe navigation：没看懂

table.move()第一种形式没看懂

- [ ] 做 table 一节的练习

# 引用语义/值语义

```lua
> a = {}
> a.name = "test"
> a.val = 10
> function modify_a(a)
>> a.name = "modfied"
>> a.val = 20
>> end
> modify_a(a)

> print(a.name, a.val)
modfied	20
> i = 100
> function modify_i(i)
>> i = 0
>> end
> modify_i(i)
> print(i)
100

> function modify_str(str)
>> str = "modified"
>> end
> str = "hello world"
> modify_str(str)
> str
hello world

```

可以发现表的引用语义，其他类型是值语义。



# 作用域



# 控制流





# 函数

```lua
function name(arg1, arg2, ... , arg n)
    - body
    return return-values
end
```

如果函数只有一个参数，且是字符串或者表构造器，可以省略`()`。如果实参数量少于形参，函数仍然可以被调用，无对应实参的参数为`nil`;如果实参数量大于形参，多余的实参被丢弃。

参数是局部变量，表是传引用语义，其他类型是传值语义，见[引用语义/值语义](#引用语义/值语义)。

## 默认参数

Lua 不支持默认参数，但是可以通过”为传递实参的参数为`nil`“的特性实现默认参数。



## 返回多个结果

不需要制定函数的返回类型和数量，允许返回任意类型和任意数量的结果。当返回值有多个时，Lua 将根据不同的场景对返回值进行”截断“。当函数调用作为表达式列表中的最后（或唯一）的表达式时，返回多个结果;当函数调用包含在表达式中时，仅返回第一个结果。

多个返回值的返回和使用其赋值的语法形式和 Rust 语言的模式匹配类似。其中没有被匹配到的值被赋为`nil`。

```lua

```

但是需要注意的是，返回多个结果就是它字面的意思，而不是返回一个列表。

## 变长参数

`table.pack()`

`table.unpack()`

`select()`

变长参数为 Lua 函数提供了极大的灵活性，实现了类似 C++ 函数重载的功能。

## 尾调用消除

原理。

识别尾调用消除的场景。

## 局部函数

# 闭包

**使用闭包重定义函数：沙盒**

# IO

Lua 定位为嵌入式语言，简洁小巧使它最重要的目标之一，所以 IO 库仅提供了部分与 C 标准库接口、功能相似的操作流的函数。如果要直接操作文件或实现更复杂的 IO，需要调用外部程序完成。

简单 IO 模型和完整 IO 模型。

所有的 IO 函数都在错误时返回`nil`并附带错误信息，可以结合`assert()`使用以确保 IO 必须成功（否则终止程序）。



## 简单 IO 模型

![](%E5%9F%BA%E7%A1%80.assets/Screenshot_20200812_220124.png)





## 复杂 IO 模型

`io.lines()`



## 调用外部程序与系统功能

`os`

`io.fopen()`

`os.execute()`

`os.getenv()`

如果要使用更多的 OS 功能，应该使用外部 Lua 库，比如 *LuaFileSystem*。





# 模式匹配



# 时间



# 数据结构



## 数组

## 链表

## 队列

## 倒置表

## 集合

## 字符串缓冲区



```lua
#!/usr/bin/env lua

local string_buf = ""
local cnt = 0
assert(io.input("Emmylua-LS-all.jar"))
-- bad code
-- 
-- for line in io.lines() do
-- 	cnt = cnt + 1
-- 	string_buf = string_buf .. line .. "\n"
-- end
--

-- good code
local t = {}
for line in io.lines() do
	cnt = cnt + 1
	t[#t + 1] = line .. "\n"
end
local s = table.concat(t)

io.write(string.format("%d lines\n", cnt));

```



```bash
kongjun@192 ~/D/c/lua> time ./string_buf.lua 
40797 lines

________________________________________________________
Executed in   46.55 secs   fish           external 
   usr time   20.09 secs    0.00 micros   20.09 secs 
   sys time   26.30 secs  395.00 micros   26.30 secs 

kongjun@192 ~/D/c/lua> time ./string_buf.lua 
40797 lines

________________________________________________________
Executed in   61.30 millis    fish           external 
   usr time   48.19 millis    0.00 micros   48.19 millis 
   sys time   12.62 millis  760.00 micros   11.86 millis 


```



## 图



# 数据文件与序列化



## 数据文件



## 数字、字符串、布尔类型和`nil`的序`do列化

`%q`

长字符串



## 表的序列化



# 编译、执行

## 预编译

## 动态执行

**Lua 5.3**

`loadfile()`：加载文件

`dofile()`：执行文件

`load()`：加载字符串作为代码

`loadfile()`和`l`dooad()`返回值是一个函数，调用该函数执行该段代码。

`load()`常见用法：返回外部值`load("return" .. external_code)`。`load()`返回的函数原型为`function (...)`是一个可变参数函数，且`load()`中代码在函数中，整个函数编译在全局环境中，使用全局变量。

`load()`和`loadfile()`可以加载预编译文件，并提供了加载文件类型的设置，比如`t`(textual),`b`(binary),`bt`(textual and binary)，默认是`bt`。



**Lua 5.1**:

`loadfile()`：编译某个文件，返回一个函数

`dofile()`：编译某个文件并运行它

`loadstring()`：编译某个字符串，返回一个函数

`load()`

编译文件和运行代码是两码事，编译文件不会产生任何副作用。

编译代码后，如果运行的话调用返回的函数即可。`loadstring()`返回的函数相当于是一个在全局环境中的可变参数的函数，字符串可以当作一般的函数体写。一般不使用`loadstring()`和`load()`

# 错误处理

`pcall()`和`xpcall()`,类似与`try ... catch ...`

对于异常（未预料的错误），简单的避免的产生`error()`;复杂的返回错误码。



# 模块和包

以下是 Lua 5.3

## 模块的查找和加载

`require(mod)`过程：

1. 查看表`package.loaded`是否有要导入的模块`mod`，如果有则返回已导入模块的 table`package.load[mod]`。
2. 如果没有导入，则通过表`package.searchers`中的四个查找器查找对应模块，并返回对应模块的 table。
3. 如果没有找到相应模块，返回`nil`和错误信息。



**查找器**



`package.searchers`

`package.preload`

`package.searchers[1]`:`通过package.preload`表返回加载器。`package.preload`表中包含对应模块的加载器，如果存在相应加载器，直接返回。

`package.searchers[2]`:查找 Lua 模块。

`package.searchers[3]`:查找 C 模块`mod`，返回加载器`luaopen_mod`。比如

`package.searchers[4]`:查找 C **子模块**，将子模块的所在的包当作自摸块。如果没有查找到`a.b.c`，但是查找到了 C 模块`a`，将其当成子模块`a.b.c`，加载器为`luaopen_a_b_c()`。

Lua 的运行环境是 ISO C，ISO C 中没有路径的概念，因此 Lua 也没有路径的概念，Lua 使用*模板*来表示“路径”。模板由多个路径组成，路径之间由`;`分割，其中模块名使用`?`代替，运行时模块名会替换`?`。

模块的搜索路径：`package.path`，`package.cpath`。这`package.path`由`LUA_PATH_5_3`、`LUA_PATH`、内置路径（Lua编译时写入）决定;`package.cpath`由`LUA_CPATH_5_3`、`LUA_CPATH`、内置路径（Lua编译时写入）决定。

```lua
> print(package.cpath)
/usr/lib64/lua/5.3/?.so;/usr/lib64/lua/5.3/loadall.so;./?.so
> print(package.path)
/usr/share/lua/5.3/?.lua;/usr/share/lua/5.3/?/init.lua;/usr/lib64/lua/5.3/?.lua;/usr/lib64/lua/5.3/?/init.lua;./?.lua;./?/init.lua
> 

```

可以使用`package.searchpath(mod, path)`在某个模板（目录）`path`中查找模块`mod`。

## 子模块

`.`在模块名中有特殊意义，在查找模块时作为目录分割符使用（比如UNIX下的`/`）。

## 定义模块



# TODO

- [ ] pipe 解决读写者问题
- [ ] 排列组合算法
- [ ] luasocket 例子
- [ ] 数据描述语言





