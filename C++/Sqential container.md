## 容器类型别名
* forward_list之外的顺序容器都支持reverse_iterator

|类型别名|介绍|
|-|-|
|iterator|容器类型的迭代器类型|
|const_iterator|容器类型的常量迭代器类型|
|reverse_iterator|反向迭代器|
|const_reverse_iterator|常量反向迭代器|
|difference_type|容器的迭代器之差的类型|
|value_type|容器中元素的类型|
|size_type|能容纳此容器类型的最大可能容量的无符号整数类型|
|reference|容器的元素的引用类型|
|const_reference|容器的常量引用类型|
* iterator系列，通过begin(),end()系列成员函数获得.。
## 容器初始化
|初始化|介绍|
|-|-|
|C c|c默认初始化。如果c为array，元素值未知|
|C c1(c2)|将c1初始化为c2的拷贝，c2类型必须完全相同|
|C c{lst}|列表初始化。对于array，lst内元素数目必须小于等于array容量|
|C c(b,e)|用两个迭代器表示范围内的值初始化容器。范围内的元素的值必须与C::value_type相容。**array不支持**。|
**一下初始化方式只有除*array*之外的*顺序容器*支持**

|||
|-|-|
|C seg(n)|seq包含n个值初始化的元素，构造函数expilicit
|C seq(n,t)|seq包含n个值为t的元素
注意:
1.C c1(c2)的初始化方式,c1、c2必须**完全相同**(类型相同、元素类型相同)
2;C c(b,e)的初始化方式,只需要b、e表示的范围内的元素的类型与容器元素类型**相容**
3.array可以看做内置数组的改良版,各种行为与内置数组类似.比如:不能修改大小,创建时需要指定大小.
```
array<int,10>

```
## 赋值与swap
## 大小和修改容量
## 添加元素
## 删除元素
## 迭代器


