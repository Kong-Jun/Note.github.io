- [ ] boolean 和其他类型的转换
- [ ] 变量的作用域
- [ ] const 究竟是规定该引用，还是引用指向的值
- [ ] prototype
- [ ] generater 中 caller 和 callee 的信息传递
- [ ] 为什么使用 generater runner 可以实现异常处理
- [ ] event 的传播
- [ ] function 和 object
- [ ] exception 的传播

单引号和双引号没有区别。

数字都是双精度浮点数。

for ... in 用于对象，不应该用于 array（得到的是 index），而且会把 array 的其他属性取出来。for ... of 用于 array，得到元素。

严格等于和 abstract equality

null/undefined, empty-string/0, number/string

\+ 既可以用于算术运算、又可以用于字符串链接。+ 运算对象存在字符串，就将数字转换为字符串进行字符串连接。

除了`undefined`、`null`、`false`、`0`、`NaN`和`''`为`false`，其他都为`true`。	

`>>`算术右移，`>>>`逻辑右移。

结构化绑定可以用于数组、对象，必须在定义时使用，单独使用需要将表达式用`()`扩起来。

```javascript
const arr = [1, 2, 3, 4, 5];
let [x, y, ...rest] = arr;
x; // 1
y; // 2
// -----------------------
const arr = [1, 2, 3, 4, 5];
let x, y;
[x ,y] = arr; // error!!!
([x, y] = arr); // correct!!!
// ----------------
let a = 5, b = 10;
[a, b] = [b, a];
a; // 10
b; // 5
```

结构化绑定还可以用在函数参数。

```javascript
function f(a, b = "default", c = 3) {
 return `${a} - ${b} - ${c}`;
}
f(5, 6, 7); // "5 - 6 - 7"
f(5, 6); // "5 - 6 - 3"
f(5); // "5 - default - 3"
f(); // "undefined - default - 3"

```

arrow 语法时函数定义的语法糖，最主要的不同在于，它有词法作用域。另外 arraw 定义的函数中没有`arguments`，也不能作为构造器使用。

函数定义可以提前，即一个作用域中的代码可以使用定义在其后的同一作用域中的函数。

函数和对象并不是完全绑定在一起的，可以使用`apply()`，`call()`和`bind()`来绑定函数和对象。

`var`定义的变量具有**函数作用域**，并且不支持**变量遮掩**，ES6 绝对不要使用`var`。在 ES5 中，没有经过`var`声明的变量默认是全局的，可以通过开启 strict mode 禁止这种行为。strict mode 通过`use strict`开启，如果在全局作用域开启 strict mode，所有脚本都会受到影响，通常不建议在全局作用域开启 strict mode，建议将整个脚本使用函数包裹起来，在这个函数中开启 strict mode。

```javascript
(function() {
 'use strict';
 // all of your code goes here...it
 // is executed in strict mode, but
 // the strict mode won't contaminate
 // any other scripts that are combined
 // with this one
})();
```

对象的 key 只能是 string 或 symbol。

`concat()`返回拷贝，不修改调用对象和参数。`slice()`使用左闭右开区间。`splice()`同时结合了插入和删除，插入返回空数组，删除范围删除的数组。

`map()`、`filter()`和`reduce()`等函数不去处理数组中未定义或已删除的元素。

异常的捕获和抛出仅限于函数内部，不可以在函数内抛出异常然后在函数外捕获。



# OOP

对象是函数

访问控制

静态方法

继承

const messyPhone = '(505) 555-1515';
const neatPhone = messyPhone.replace(/\D/g, '');    所有对象都继承自`Object`。`toString()`

多态







函数是一个对象！继承自`Function`。

typeof 数组，结果是 object





character set: \d \D \w \W \s \W

repetition: {n, m} {n,} {n} ? + *

caputure group `($<Name>pattern)`    uncapture group `($:pattern)`

grouping replace     $n ... \$` \$' \$&

默认贪婪匹配，在 repetition 后面添加`?`改为 lazy matching。

anchoring ^ $ 对于字符串，其中的`\n`当作普通字符处理，不当成行尾，如果要当成行尾处理，在 regex 后添加`m`(multiline)标志。

先行`(?=)`、后向`(?<)`及其取反`(?!=)`/`(?!<)`

使用`replace()`和函数，先在`replace()`中匹配一段，再在函数中进行更精细地匹配。

利用 template string 动态构造 regex 





DOM 增删改查 修改style

Event

Ajax

jQuery

