# iostreamVimspector配置与使用

[vimspector](https://github.com/puremourning/vimspector#supported-languages)是一个基于*DAP(debug adapter protocol)*的Vim多语言调试插件，理论上能够支持所有支持DAP的语言。这个插件仍在实验阶段，可能会有各种bug，但是对C/C++、Python的支持已经进行了充分的测试，可以放心使用。

[TOC]

## 安装

使用`vim-plug`安装：

```
Plug 'puremourning/vimspector'
```

安装完成后进入`vimspector`的安装目录，执行：

```bash
./install_gadget.py <language-name>
```

`install_gadget.py`会自动下载`<language-name>`所需的调试适配器并进行相应配置,`--help`可以查看`vimspector`所支持的全部语言。

以在Linux环境上打开C/C++支持为例：

```
./install_gadget.py --enable-c
```

`vimspector`会自动下载微软开发的调试适配器`cpptools-linux.vsix`到`your-vimspector-path/gadgets/linux/download/vscode-cpptools/0.27.0/`中。如果是在mac上，`linux`会被改成`mac`。

因为这个文件的下载地址可能无法在国内访问，可以使用VPN手动下载好放置在上面提到的目录中，然后再执行以上命令。

## 配置

`vimspector`使用json作为配置文件的格式，每个配置都是一个json对象。

使用`vimspector`时必须先写好配置文件，`vimpector`有两类配置：

- **调试适配器的配置**
  - 如何启动或连接到调试适配器
  - 如何attach到某进程
  - 如何设置远程调试
- **调试会话的配置**
  - 使用哪个调试适配器
  - launch或attach到进程
  - 是否预先设置断点，在何处设置断点

这两类配置可以对应多个配置文件，`vimspector`会将多个配置文件中的信息合并成一个配置。

### 调试适配器配置

调试适配器的这个配置在打开`vimspector`对某语言的支持时就已经自动设置好了，存放在`your-path-to-vimspector/gadgets/linux/.gadgets.json`中。

比如在我的设备上，`.gadgets.json`内容如下：

```json
{
  "adapters": {
    "vscode-cpptools": {
      "attach": {
        "pidProperty": "processId",
        "pidSelect": "ask"
      },
      "command": [
        "${gadgetDir}/vscode-cpptools/debugAdapters/OpenDebugAD7"
      ],
      "name": "cppdbg"
    }
  }
}

```

其中变量`${gadgetDir}`代表着存放`.gadgets.json`的目录。除此之外，`vimspector`还定义了其他预定义变量，并提供了自定义和用户输入变量内容的功能，以便我们编写比较通用的配置文件。

调试适配器的配置还可以存在于其他配置文件中，`vimspector`读取一系列配置文件，生成`adapters`对象。

调试适配器的配置可以存在于以下文件中：

1. `your-path-to-vimspector/gadgets/<os>/.gadgets.json`：这个文件时`vimspector`自动生成的。

2. ``your-path-to-vimspector/gadgets/<os>/.gadgets.d/*.json`：这个文件是用户自定义的。

3. 在Vim打开的文件的父目录中递归搜索到的第一个`.gadgets.json`。

4. `.vimspector.json`中定义的`adapters`。

编号代表配置文件的优先级，编号越大优先级越高，高优先级的配置文件将覆盖低优先级的配置文件中的的`adapters`。

在我的机器上没有`your-path-to-vimspector/gadgets/<os>/.gadgets.d`目录，可能是需要自己创建。

### 调试会话配置

项目的调试会话的文件位于以下两个位置：

1. `<your-path-to-vimspector>/configurations/<os>/<filetype>/*.json`
2. 项目文件中的`.vimspector.json`

每当打开一个新的调试会话时，`vimspector`都会在当前目录向父目录递归搜索，如果查找到了`.vimspector.json`，则使用其中的配置，并将其所在的目录设定为项目根目录;如果未查找到，则使用`vimspector`安装目录中的配置文件，将打开的文件的目录设置为项目根目录。

奇怪的是，`vimspector`文档中描述的全局配置所在的目录并不存在，当我自己创建相应的目录、文件后，`vimspector`报告找不到调试设置。我不得已只能在每个项目根目录中创建单独的`.vimsspector.json`。

### 配置选项

`.vimspector.json`中只能包含一个对象，其中包含以下结构：

- `adapters`：调试适配器配置，不建议包含
- `configurations`：调试程序时的配置

`configurations`应该包含以下关键字：

- `adapter`:使用的调试配置器名称，该名称必须出现在`adapters`块或其他调试适配器配置中。

- `variables`：用户定义的变量
- configuration：调试器实际使用的配置
- `remote-request`,`remote-cmdLine`:远程调试使用

`configuration`必须包含以下两个关键字：

- `request`：调试的类型，`lauch`或`attach`
- `type`：

除了以上的选项，还应该设置要调试的程序的路经、参数、环境变量、调试器等。

上面的选项构成了配置文件的主体框架，更详细的配置选项可以参考[](https://puremourning.github.io/vimspector/schema/)

## 变量

`vimspector`为了增强配置的通用型，降低编写难度，比较丰富的变量。

### 预定义变量

![预定义变量](https://img2020.cnblogs.com/blog/1954702/202005/1954702-20200505171059800-1196625274.png)

### 自定义变量

可以在`variable`中定义变量。

```javascript
{
  "configurations": {
    "some-configuration": {
      "variables": {
        "gdbserver-version": {
          "shell": [ "/path/to/my/scripts/get-gdbserver-version" ],
          "env": {
            "SOME_ENV_VAR": "Value used when running above command"
          }
        },
        "some-other-variable": "some value"
      }
    }
  }
}
```

其中`gdbserver-version`和`some-other-variable`都是用户定义的变量，可以像预定义变量一样使用。

可以调用外部命令，将外部命令的输出赋给变量。`gdbserver-version`的值就是`/path/to/my/scripts/get-gdbserver-version`的输出。

还可以在运行`vimspector`时输入变量的值。最典型的运例子是程序参数的传递，`vimspector`调试的程序的参数以数组的形式传递，在配置文件中将`args`设置为一个在运行时用户输入的变量，就可以模拟命令行的效果。

用户输入值的变量用`"*${variable-neme}`表示，比如以下配置：

``` javascript
  "args": [ "*${CommandLineArgs}" ]
```

在运行时`vimspector`会要求用户输入值，如果用户输入`1`、`2`、`3`,`args`就会被拓展成`["1", "2", "3"]`传递给程序。

## 示例

### 调试vim

`vimspector`文档中给出的调试vim的配置：

```javascript
{
  "configurations": {
    "Vim - run a test": {
      "adapter": "vscode-cpptools",
      "configuration": {
        "type":    "cppdbg",
        "request": "launch",
        "program": "${workspaceRoot}/src/vim",
        "args": [
          "-f",
          "-u", "unix.vim",
          "-U", "NONE",
          "--noplugin",
          "--not-a-term",
          "-S", "runtest.vim",
          "${Test}.vim"
        ],
        "cwd": "${workspaceRoot}/src/testdir",
        "environment": [
          { "name": "VIMRUNTIME", "value": "${workspaceRoot}/runtime" }
        ],
        "externalConsole": true,
        "stopAtEntry": true,
        "MIMode": "lldb",
        "logging": {
          "engineLogging": false
        }
      }
    }
  }
}

```

### 我的配置

定义了`launch`和`attach`两种类型，进入`vimspector`后会提示选择类型。如果是`launch`则提示输入参数，如果时`attach`则提示输入PID。

```javascript

{
  "configurations": {
    "cpp:launch": {
       "adapter": "vscode-cpptools",
       "configuration": {
         "name": "cpp",
         "type":    "cppdbg",
         "request": "launch",
         "program": "${fileDirname}/${fileBasenameNoExtension}",
		 // user input args
		 "args": ["*${ProgramArgs}"],
         "cwd": "${workspaceRoot}",
         "environment": [],
         "externalConsole": true,
         "stopAtEntry": true,
         "MIMode": "gdb",
         "logging": {
           "engineLogging": false
         }
      }
    },
    "cpp:attach": {
       "adapter": "vscode-cpptools",
       "configuration": {
          "name": "cpp",
          "type": "cppdbg",
          "request": "attach",
          "program": "${fileDirname}/${fileBasenameNoExtension}",
          "MIMode": "gdb"
       }
    }
  }
}

```

运行截图：

![](https://img2020.cnblogs.com/blog/1954702/202005/1954702-20200505174736410-49444089.png)

选择2后：

![](https://img2020.cnblogs.com/blog/1954702/202005/1954702-20200505175013716-1618383575.png)

## 使用

`vimspector`预设了vscode mode和human mode两套键盘映射（快捷键）。

开启vscode mode：

```
let g:vimspector_enable_mappings = 'VISUAL_STUDIO'
```

![](https://img2020.cnblogs.com/blog/1954702/202005/1954702-20200505175802855-63918029.png)



开启human mode：

```
let g:vimspector_enable_mappings = 'HUMAN'
```

![](https://img2020.cnblogs.com/blog/1954702/202005/1954702-20200505175855111-809170906.png)



也可以使用`vimspector`提供的函数，自己定义映射。