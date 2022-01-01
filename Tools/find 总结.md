

## `-prune`和 find 的默认行为

find 命令的语法如下语法：`find [path] [expression] [action]`。find 递归地在`path`搜索满足`expression`的文件/目录，并在其上执行`action`。

默认情况下（不加`-print`)，find 会将所有的的`expression`**与**起来，并执行`-print`。

find 的默认行为看起来很正常，如打印所有文件名以`test`结尾的文件：

```
find -name '*test` -type f
```

但当使用`-prune`排除某目录时，默认行为就会产生奇怪的结果。如打印`images`目录外的`.jpg`文件：

```
find . -type d -name images -prune -o -name '*.jpg'
```

执行后会发现将`images`目录被打印了出来，这和我们的目的相悖，罪魁祸首就是 find 的默认行为。这个命令等价于：

```
find . \(-type d -name images -prune -o -name '*.jpg'\) -print
```

圆括号在 shell 中有特殊意义所以需要转义。`-prune`是一个`action`而不是`expression`，它的作用指示 find 不要进入该目录搜索，并且它总是返回`false`。

上面的命令括号中的条件匹配到了`images`和所有`images`外的以`.jpg`结尾的文件/目录，然后将它们打印了出来。

为了避免 find 的默认行为在使用`-prune`时导致结果异常，需要显式地指定`action`。上面的命令应该改写成：

```
find . -type d -name images -prune -o -name '*.jpg' -print
```

这个命令中的`-type d -name images`匹配到了目录`images`，但是`-prune`命令指示不要进入其中搜索，因为没有加`-print`，因此虽然匹配到了目录`images`但不会打印它，`-prune`返回`false`，执行`-name '*.jpg' -print`将所有不再`images`目录中的 jpg 文件名打印出来

## `-exec`和文件列表

`-exec`选项中可以使用`{}`表示匹配到的文件名，命令结尾必须加`\;`。使用`{}`时，find 每次匹配到一个文件，就会产生一个输出，并将它写入`{}`。 有时我们执行的命令需要接受一个文件列表而不是单个文件，这时可以使用`{} +`，命令尾部不可以加`\;`。每当 find 产生一个输出，并把它添加到`{} + `尾部。

使用命令`echo`查看`{}`和`{} + `的区别：

```shell
SHELL>ls
1  2  3
SHELL> find . -exec echo {} \;
.
./1
./2
./3
SHELL> find . -exec echo {} + 
. ./1 ./2 ./3
```

可见匹配到的文件名被存了起来然后一次性输出到` {} +`。

`{} + `有时仍不够灵活，这时可以使用管道和`xargs`从 find 的输出构造文件列表。



## find 处理文件和目录的顺序

find 递归搜索目录时先处理目录自身，再处理目录条目（目录中f的文件），可以通过指定`-depth`选项来强制 find 最后处理目录自身。当`-depth`选项被指定时，`-prune`无效。`-delete`隐含了`-depth`，因此`-delete`不能和`-prune`同时使用。