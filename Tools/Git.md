# git操作

- [ ] ` git rebase`的机制究竟是怎样的？最终的结果与`git merge`有什么区别？为什么上游分支存在的跟变基分支相同的变化不会被应用?`rebase`为什么会发生冲突？变基后的提交对象跟原来的对象有什么区别？
- [ ] `git log`的高级选项
- [ ] `git merge`的多种策略

# 创建/克隆仓库

情景描述：在本地创建了一个仓库，又在 Gitee 上创建了一个仓库作为该仓库的远程仓库，在`git push`时提示`fatal: 拒绝合并无关的历史`，使用一下命令关联远程分支并合并无关历史。

```sh
git branch --set-upstream-to=origin/master master

git fetch
git merge --allow-unrelated-histories  
```



# 删除/改名

`git rm`

`git rm --staged`和`git restore --staged`没有区别

# 撤销修改

`git restore <file>`:

`git restore --staged <file>`

# 暂存/提交

使用`git commit`进入交互模式提交，或者使用`git commit -m`添加评论并提交。注意，`commit`是将暂存区中的内容提交到版本库中，如果工作区中的文件做个多次修改但只并为全部暂存https://gitee.com/progit/3-Git-%E5%88%86%E6%94%AF.html#3.6-%E5%88%86%E6%94%AF%E7%9A%84%E8%A1%8D%E5%90%88，就会出现一部分修改暂存，一部分未跟踪的窘境，提交后版本库中仅获得了部分修改。为跟踪的修改在工作区中，所以提交后看起来是正常的，但是版本库中没有该内容。这种情况应该避免。

# `.gitignore`

`.gitignore`中的 glob 模式匹配到的文件/目录会被忽略（不被跟踪）。`.gitignore`使用的是 glob 的语法子集，并添加了取反`!`。

`*`：匹配所有不包含`/`的任意文件/目录名

`**`: 匹配所有文件/目录名

`!pattern:匹配所有未匹配到`pattern`的文件/目录

`.gitignore`不支持`[]`和`{}`的语法。

# 查看提交历史

`git log`默认只能看到当前分支之前的提交，如果需要查看所有提交（获取 Git 分支的全貌），可以加上`--all`选项。

# 标签

`git tag -s <tag>`

`git tag -l <tag>`

`git tag -a <tag>`

`git tag <tag>`

`git show <tag>`

`git tag -d <tag>`



# 分支

分支仅仅是一个指向提交对象的指针（`sha`串），对分支的所有操作都不影响提交对象。

Git 中的所有对象都使用`sha1`哈希值索引，Git 通过这个值来判断两个文件是否相同，删除分支后

## 本地分支

## 远程分支

## 合并

```shell
git-merge - Join two or more development histories together

git merge [-n] [--stat] [--no-commit] [--squash] [--[no-]edit]
               [--no-verify] [-s <strategy>] [-X <strategy-option>] [-S[<keyid>]]
               [--[no-]allow-unrelated-histories]
               [--[no-]rerere-autoupdate] [-m <msg>] [-F <file>] [<commit>...]
 git merge (--continue | --abort | --quit)
```

`git merge`默认使用“三方合并”的策略，它根据当前节点、欲并入的节点和两个节点的祖先来确定最终合并的结果。两分支的祖先可以使用`git merge-base <branch1> <branch2>`查看。如果没有冲突，就创建一个新提交对象，并且它的父亲`git merge`作用的那两个节点;如果有冲突，使用该手动解决冲突后再合并。

产生了冲突后，如果希望回退回原来的状态，可以使用`git merge --abort`。如果解决了冲突，可以使用`git merge --continue`继续合并。

合并保留了全部的历史，便于回溯，但是会产生混乱的分支，如果希望产生完美的线性分支就要用到`git rebase`了。

## 变基

```shell
git-rebase - Reapply commits on top of another base tip

git rebase [-i | --interactive] [<options>] [--exec <cmd>] [--onto <newbase>]
        [<upstream> [<branch>]]
git rebase [-i | --interactive] [<options>] [--exec <cmd>] [--onto <newbase>]
        --root [<branch>]
git rebase --continue | --skip | --abort | --quit | --edit-todo | --show-current-patch
```

变基是 Git 的魔法，它允许我们直接修改、调整分支的形状，给了我们产生完美的线性分支的机会。

**变基的黄金法则**：永远不要在公开的分支上使用变基

`git rebase`会删除旧的提交对象，产生新的提交对象，如果在公开的（可能会被别人访问的）的分支上使用变基，当别人克隆了变基前的仓库时，再`git pull`会将变基后的对象当做新的修改进行合并，导致分支非常混乱。在发起 pull request 后进行变基也会导致问题，当别人查看该分支的代码时会发现该分支凭空消失。总之，所有的需要跟他人交互的分支都不能使用变基，变基只能发生在跟他人交互之前。

关于变基的更详细的资料见[merging vs rebasing](https://www.atlassian.com/git/tutorials/merging-vs-rebasing)。



**本地分支清理**

`task2.3`分支先是完成了该练习的代码，然后又通过一次提交补上了 README。我希望让该分支保持非常干净的状态，不希望让“补上了 README" 这种本该在上一次提交中完成的事情作为独立的提交存在。

可以使用`git rebase`的交互模式达到这个目的，将`task2.3`变基到`test2`，然后再交互模式的编辑器中，将`2a2dbe`的提交改为`fixup`（意味着该提交会”融入”前面的`b156d94`），使两者合二为一。

```shell
[I] kongjun@localhost ~/D/c/linux-0.12 (task2.3)> git log --all --graph --abbrev-commit --pretty=oneline
* 2ba2dbe (HEAD -> task2.3) 添加了README
* b156d94 完成了task2.3
| * db68393 (master) 添加了.gitignore文件，删除了test1-homework。被删除的homework中包含的每一个task都将建立一个独立的分支。 master分支只添加注释，不进行其他修改。 还应该为每个test章节创建test分支，每个test分支只添加该部分代码的注释，并将master前进到该分支。 还应该从master（test）为每个task创建task分支，再其上进行各种练习。
|/  
| * 24d9b78 (task2.5) 完成了task2.5
| * e05ae82 (task2.4) 完成了task2.4
|/  
| * ef419e6 (task2.2) 完成了task2.2
|/  
* 9361bbf (test2) 添加了test2分支，其中仅添加了注释。task再创建新的分支
| * 36aadc1 (task2.1) 完成了task2.1。由于task1.2原本就使用了循环，所以task2.1的代码和task1.2一样
| | * 8828a37 (task1.2) 尝试使用shell脚本创建分支
| |/  
| | * 95ee56a (task1.3) 尝试使用shell脚本创建分支
| |/  
| | * fe23176 (task1.4) 尝试使用shell脚本创建分支
| |/  
| | * df2d727 (task1.5) 尝试使用shell脚本创建分支
| |/  
| * e1f5502 (task1.1) 添加了.gitignore文件，删除了test1-homework。被删除的homework中包含的每一个task都将建立一个独立的分支。 master分支只添加注释，不进行其他修改。 还应该为每个test
章节创建test分支，每个test分支只添加该部分代码的注释，并将master前进到该分支。 还应该从master（test）为每个task创建task分支，再其上进行各种练习。
| * ae56086 添加了test2分支，其中仅添加了注释。task再创建新的分支
|/  
* 0700765 读完了test2
* fee301e 清除了test1 中task*的文件，但保留了homework目录
* 9ae2d13 init
[I] kongjun@localhost ~/D/c/linux-0.12 (task2.3)> git rebase -i test2 task2.3
Successfully rebased and updated refs/heads/task2.3.
[I] kongjun@localhost ~/D/c/linux-0.12 (task2.3)> git log --all --graph --abbrev-commit --pretty=oneline
* 5977c4f (HEAD -> task2.3) 完成了task2.3
| * db68393 (master) 添加了.gitignore文件，删除了test1-homework。被删除的homework中包含的每一个task都将建立一个独立的分支。 master分支只添加注释，不进行其他修改。 还应该为每个test章节创建test分支，每个test分支只添加该部分代码的注释，并将master前进到该分支。 还应该从master（test）为每个task创建task分支，再其上进行各种练习。
|/  
| * 24d9b78 (task2.5) 完成了task2.5
| * e05ae82 (task2.4) 完成了task2.4
|/  
| * ef419e6 (task2.2) 完成了task2.2
|/  
* 9361bbf (test2) 添加了test2分支，其中仅添加了注释。task再创建新的分支
| * 36aadc1 (task2.1) 完成了task2.1。由于task1.2原本就使用了循环，所以task2.1的代码和task1.2一样
| | * 8828a37 (task1.2) 尝试使用shell脚本创建分支
| |/  
| | * 95ee56a (task1.3) 尝试使用shell脚本创建分支
| |/  
| | * fe23176 (task1.4) 尝试使用shell脚本创建分支
| |/  
| | * df2d727 (task1.5) 尝试使用shell脚本创建分支
| |/  
| * e1f5502 (task1.1) 添加了.gitignore文件，删除了test1-homework。被删除的homework中包含的每一个task都将建立一个独立的分支。 master分支只添加注释，不进行其他修改。 还应该为每个test
章节创建test分支，每个test分支只添加该部分代码的注释，并将master前进到该分支。 还应该从master（test）为每个task创建task分支，再其上进行各种练习。
| * ae56086 添加了test2分支，其中仅添加了注释。task再创建新的分支
|/  
* 0700765 读完了test2
* fee301e 清除了test1 中task*的文件，但保留了homework目录
* 9ae2d13 init

```



**使用场景1**

理想的分支：每个`test`分支包含添加了注释的代码，在`test`分支上添加`task`分支完成每道练习题。`master`分支包含全部的添加了注释的代码。

现在从`master`分支创建了`task1.*`分支，并且从`master`分支的上游创建了`test2`分支，`test2`分支和`master`分支内容基本是一样的。我需要将`master`分支变基到`test2`后以创造理想的分支形状。

```shell
[I] kongjun@localhost ~/D/c/linux-0.12 (master)> git log --all --graph --abbrev-commit --pretty=oneline
* d32ffa2 (task2.3) 添加了README
* b156d94 完成了task2.3
| * 24d9b78 (task2.5) 完成了task2.5
| | * 64819be (refs/stash) WIP on task2.5: e05ae82 完成了task2.4
| |/| 
| | * 7e065eb index on task2.5: e05ae82 完成了task2.4
| |/  
| * e05ae82 (task2.4) 完成了task2.4
|/  
| * ef419e6 (task2.2) 完成了task2.2
|/  
* 9361bbf (test2) 添加了test2分支，其中仅添加了注释。task再创建新的分支
| * 36aadc1 (task2.1) 完成了task2.1。由于task1.2原本就使用了循环，所以task2.1的代码和task1.2一样
| | * 8828a37 (task1.2) 尝试使用shell脚本创建分支
| |/  
| | * 95ee56a (task1.3) 尝试使用shell脚本创建分支
| |/  
| | * fe23176 (task1.4) 尝试使用shell脚本创建分支
| |/  
| | * df2d727 (task1.5) 尝试使用shell脚本创建分支
| |/  
| * e1f5502 (HEAD -> master, task1.1) 添加了.gitignore文件，删除了test1-homework。被删除的homework中包含的每一个task都将建立一个独立的分支。 master分支只添加注释，不进行其他修改。 
还应该为每个test章节创建test分支，每个test分支只添加该部分代码的注释，并将master前进到该分支。 还应该从master（test）为每个task创建task分支，再其上进行各种练习。
| * ae56086 添加了test2分支，其中仅添加了注释。task再创建新的分支
|/  
* 0700765 读完了test2
* fee301e 清除了test1 中task*的文件，但保留了homework目录
* 9ae2d13 init
[I] kongjun@localhost ~/D/c/linux-0.12 (master)> git rebase test2 master
丢弃 ae56086dce83b49f1bcfd1c4d81a9e6420c6a0b1 添加了test2分支，其中仅添加了注释。task再创建新的分支 -- 补丁内容已在上游
Successfully rebased and updated refs/heads/master.
[I] kongjun@localhost ~/D/c/linux-0.12 (master)> git log --all --graph --abbrev-commit --pretty=oneline
* db68393 (HEAD -> master) 添加了.gitignore文件，删除了test1-homework。被删除的homework中包含的每一个task都将建立一个独立的分支。 master分支只添加注释，不进行其他修改。 还应该为每个test章节创建test分支，每个test分支只添加该部分代码的注释，并将master前进到该分支。 还应该从master（test）为每个task创建task分支，再其上进行各种练习。
| * d32ffa2 (task2.3) 添加了README
| * b156d94 完成了task2.3
|/  
| * 24d9b78 (task2.5) 完成了task2.5
| | * 64819be (refs/stash) WIP on task2.5: e05ae82 完成了task2.4
| |/| 
| | * 7e065eb index on task2.5: e05ae82 完成了task2.4
| |/  
| * e05ae82 (task2.4) 完成了task2.4
|/  
| * ef419e6 (task2.2) 完成了task2.2
|/  
* 9361bbf (test2) 添加了test2分支，其中仅添加了注释。task再创建新的分支
| * 36aadc1 (task2.1) 完成了task2.1。由于task1.2原本就使用了循环，所以task2.1的代码和task1.2一样
| | * 8828a37 (task1.2) 尝试使用shell脚本创建分支
| |/  
| | * 95ee56a (task1.3) 尝试使用shell脚本创建分支
| |/  
| | * fe23176 (task1.4) 尝试使用shell脚本创建分支
| |/  
| | * df2d727 (task1.5) 尝试使用shell脚本创建分支
| |/  
| * e1f5502 (task1.1) 添加了.gitignore文件，删除了test1-homework。被删除的homework中包含的每一个task都将建立一个独立的分支。 master分支只添加注释，不进行其他修改。 还应该为每个test
章节创建test分支，每个test分支只添加该部分代码的注释，并将master前进到该分支。 还应该从master（test）为每个task创建task分支，再其上进行各种练习。
| * ae56086 添加了test2分支，其中仅添加了注释。task再创建新的分支
|/  
* 0700765 读完了test2
* fee301e 清除了test1 中task*的文件，但保留了homework目录
* 9ae2d13 init

```



**see also**:

[Git Manual#git-rebase](https://manpages.debian.org/buster/git-man/git-rebase.1.en.html)

   	[merging vs rebasing](https://www.atlassian.com/git/tutorials/merging-vs-rebasing)

[Pro Git#分支的衍合](https://gitee.com/progit/3-Git-%E5%88%86%E6%94%AF.html#3.6-%E5%88%86%E6%94%AF%E7%9A%84%E8%A1%8D%E5%90%88)



# 储藏

```shell
NAME
       git-stash - Stash the changes in a dirty working directory away

SYNOPSIS
       git stash list [<options>]
       git stash show [<options>] [<stash>]
       git stash drop [-q|--quiet] [<stash>]
       git stash ( pop | apply ) [--index] [-q|--quiet] [<stash>]
       git stash branch <branchname> [<stash>]
       git stash [push [-p|--patch] [-k|--[no-]keep-index] [-q|--quiet]
                    [-u|--include-untracked] [-a|--all] [-m|--message <message>]
                    [--pathspec-from-file=<file> [--pathspec-file-nul]]
                    [--] [<pathspec>...]]
       git stash clear
       git stash create [<message>]
       git stash store [-m|--message <message>] [-q|--quiet] <commit>

```

`git stash`储藏所做的修改（将其存储到 stash 堆栈中），使当前分支变干净，可以正常切换分支，在合适的时候将储藏的修改应用到某个分支，使其恢复原状。

`git stash`默认没有用户自定义的信息，如果需要添加一些信息让 stash 堆栈更清晰易读可以使用`git stash save [<message>]`，man pages 中建议使用`git stash push -m <message>`。

`git apply`将栈顶的修改恢复到当前分支，并且不将其出栈。如果需要将堆栈中的其他储藏出栈可以显式地指定要恢复的储藏`git stash [<stash>]`。如果需要将其出栈，可以使用` git drop <stash>`或直接使用`git stash pop`（将栈顶出栈并应用）。

`git stash`默认不储藏未跟踪的文件，如果需要的储藏未跟踪的文件，可以指定`--include-untracked`选项，还可以指定`--no-keep-index`不储藏暂存区。`git apply`默认不恢复暂存区，因此会忽略暂存区的冲突，如果要恢复暂存区需要指定`--index`选项;`git pop`默认恢复暂存区，如果暂存区发生冲突将终止应用。

`git stash`的一个使用场景是：在当前分支上进行了修改，但现在要切换到某个分支，并且不希望提交未完成的工作。这时可以使用`git stash`储藏自己的工作，切换到别的分支工作，完成该分支工作后再回到原来的分支并恢复自己当时做的修改。

`git stash branch`从当前分支创建一个分支，并将栈顶的储藏应用于这个分支。这个命令给了我们将一个分支上的修改移动到一个新的分支的机会。

在 Linux 0.12 的实验中遇到了`git branch task2.2`的使用场景。

在`test2`分支上做了修改，这些修改是为作业 `task2.2`做的，这些修改应该放在在单独的`task.2.2`分支中，而不是`test2`分支。使用一下命令储藏`test2`分支上的修改，并从储藏创建`task2.2`分支。

```shell
git stash
git stash branch task2.2
```

因为`task2.2`分支和`test2`分支指向的是一个提交对象，通过`git status`等命令可以看到两个分支共用一个对象，在`git commit`时需要注意当前分支应该是希望分支被修改的哪个分支（这里是`task2.2`）。

```shell
git commit -m "..." 
```



# 参考

[Atlassian Bitbucket](https://www.atlassian.com/git): 非常好的网站，上面有高质量的教程和文章。

[Pro Git](https://gitee.com/progit/index.html)：Git 官方教程的中文版。

[cherry pick教程](http://www.ruanyifeng.com/blog/2020/04/git-cherry-pick.html)



