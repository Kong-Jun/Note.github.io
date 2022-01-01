从 vim 命令行执行 Lua 代码：luado/lua/luafile

从 vimscript 访问 Lua 函数：`v:lua.<function>`，这种方法只能调用全局 Lua 函数，或通过 require 调用，不能将 Lua 函数转换为 Funcref。

`vim.NIL`相当于 vimscript 的 `v:null`，Lua 中的 `nil`在数组或表中转换为 vimscript 后被当作不存在。Lua 表中有数字键时，转换为 vimscript 后当成 list，非数字键忽略。Lua 空表被转换为 vimscript 空表。通过在表中设置键`vim.type_idx`可以强制在转换存在模糊的情况下转换为特定类型。`vim.val	_idx`标志表中数字是`v:Float`。`vim.type`用于`vim.type_idx`和`vim.val_idx`。

通过`vim`从 Lua 和 Vim 交互

`vim.loop`

`vim.highlight`

`vim.regex`

`vim.diff`

`vim.mpack`



从 Lua 访问 Vimscript 函数：`vim.call({func}, {...})`，`vim.fn.{func}({...})`。

从 Lua 执行 Vim 命令：`vim.cmd({cmd})`。

从 Lua 访问 Vim 变量：`vim.g`、`vim.b`、`vim.w`、`vim.t`、`vim.v`、`vim.env`。

从 Lua 访问 Vim 选项：

lua            command      global_value       local_value
vim.o           :set                set                set
vim.bo/vim.wo   :setlocal            -                 set
vim.go          :setglobal          set                 -

还可以使用`vim.opt`访问选项，通过成员函数`prepend()`、`append()`和`remove()`代替 Vim 的`set^=`、`set+=`和`set-=`。

Neovim 添加了许多 Lua 模块以增强 Lua 功能，这些函数都通过`vim`访问，如 lua-vim 添加了`tbl_isempty({t})`、`validate()`，lua-ui 添加了用户输入、选择。