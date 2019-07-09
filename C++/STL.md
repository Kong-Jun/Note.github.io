# STL顺序容器
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

以下初始化方式所有容器均支持

|初始化|介绍|
|-|-|
|C c|c默认初始化。如果c为array，元素值未知|
|C c1(c2)|将c1初始化为c2的拷贝，c2类型必须完全相同|
|C c{lst}|列表初始化。对于array，lst内元素数目必须小于等于array容量|
|C c(b,e)|用两个迭代器表示范围内的值初始化容器。范围内的元素的值必须与C::value_type相容。**array不支持**。|

**以下初始化方式只有除*array*之外的*顺序容器*支持**

|||
|-|-|
|C seg(n)|**string不支持**.seq包含n个值初始化的元素，构造函数expilicit.|
|C seq(n,t)|seq包含n个值为t的元素|

注意:
1.C c1(c2)的初始化方式,c1、c2必须**完全相同**(类型相同、元素类型相同)
2;C c(b,e)的初始化方式,只需要b、e表示的范围内的元素的类型与容器元素类型**相容**

```
vector<int> vec1={1,2,3,4,5};
vector<double> vec2(vec1);   //正确
vector<double> vec3(vec1.cbegin(),vec1.cend()); //错误
```
3.array可以看做内置数组的改良版,各种行为与内置数组类似.比如:不能修改大小,创建时需要**指定大小**.
```
array<int,10> ia1; //
array<int,10> ia1={1,2,3,4,5,6,7,8,9,10};
array<int,10> ia2(ia2);
```
## 赋值与swap
* 赋值运算符将左边容器中的元素**替换**为右边容器中元素的**拷贝**.容器中原来的元素消失,因此赋值操作后,指向原来容器元素的指针、迭代器、引用全部**失效**.
* swap(除array、string外）**交换**两个容器中元素,即交换两个容器中的**内部数据结构**,,而不是替换元素的值.swap操作后,指向原来容器元素的指针、迭代器、引用仍**有效**,但此时元素已在另一个容器中了.
* array,string特殊.当对array、string进行swap操作后,容器两容器中的元素并不交换,元素的**值交换**.指针、迭代器、引用仍指向原来的元素（原来的位置），但值已改变.

**举例说明赋值与swap的特点**

        容器就是一个装满信件的袋子,信件就是容器的元素.现有容器A、B.
        A=B,相当与扔掉A袋子中的信件,然后将B袋子中的信件的复印件替代A袋子中的信件.
        swap(A,B).  当A,B不是array或string时,相当将A袋子里的信件放进B袋子,B袋子原来的信件放进A袋子.
                    当A,B是array或string时,相当与将A袋子中信件的内容擦去,对应地写上B袋子中信件的内容;B袋子的信件也将内容擦去,对应地写上原来A信件中的内容.
            
## 访问元素
1.通过**解引用迭代器**访问元素.
2.通过**容器定义的**访问操作访问元素
3.访问**不存在的元素**,行为**未定义**
* 除了**froward_list**不支持back()操作,其他顺序容器都支持front(),back()
* 只有支持**随机访问**的容器才支持下标操作

|访问操作|说明|
|-|-|
|c.back()|返回c的尾元素的引用|
|c.front()|返回c的首元素的引用|
|c[n]|返回c中下标为n的元素的引用|
|c.at[n]|返回c中下标为n 的元素的引用.当此元素不存在时,抛出out_of_range异常|

注意不要访问不存在的元素

```
list<int> lst;
vector<int> vec;
cout<<lst.front()<<endl; //错误,lst为空容器,没有首元素.
vec[0]=1; //错误.vec是空向量,没有下标为0的元素.
```
## 添加元素

1.添加元素用insert或emplace
2.forward_list不支持back相关的操作.string、vector不支持front相关的操作
3.forward_list有特殊的insert和emplace操作
4.array不能改变容量，因此不能添加元素

|插入操作|说明|
|-|-|
|c.push_back(t)|在尾部创建一个值为t创建的元素.返回void.| 
|c.emplace_back(args)|在尾部用args创建元素.返回void.|
|c.push_front(t)|在头部创建一个值为t创建的元素.返回void.|
|c.emplace_front(args)|在头部用args创建元素.返回void.|
|c.insert(p,t)|在迭代器p**之前**的位置插入参数表示范围内的值，返回指向**第一个**插入的元素的迭代器,若参数表示范围为0,返回p.|
|c.insert(p,n,t)|同上|
|c.insert(p,b,e)|同上|
|c.insert(p,il)|同上|
|c.emplace(p,args)|在迭代器p之前插入用args构造的元素,返回新添加元素的迭代器.|
|-----------------------------|---------------------------------------------------------------------------------------------|
|lst_insert_after(p,t)|在迭代器p**之后**的位置插入参数表示范围内的值,返回**最后一个插入**的元素的值.当参数表示范围为0时,返回p;p是尾后迭代器时,行为**未定义**.|
|lst_insert_after(p,n,t)|同上|
|lst.insert_after(p,b,e)|同上|
|lst.insert(p,il)|同上|
|lst.emplace(p,args)|同上|

* 可以利用insert操作的返回值进行连续的插入.

```
vector<string> message;
vector<string>::iterator iter=message.begin();
string word;
for(cin>>word)
    iter=message.insert(iter,word) //insert操作返回第一个插入的元素的迭代器,然后再在第一个插入的元素之前插入元素. 相当与message.push_front(word).当然,vector不支持push_front操作.
```

* 某些容器类型不支持专门的在某个位置上的操作,并不意味着就不能在该位置插入.

```
//vector不支持push.front()操作,但可以用insert完成相同的功能,但是性能可能很差.其他容器类型也如此.
vector<string> vec;
vec.insert(vec.begin(),"Yes");
```

## 删除元素
1.删除部分元素用erase(),清空容器clear();
2.array不能改变容量，因此不支持删除操作;
3.forward_list不支持back相关的操作.string、vector不支持front相关的操作;
4.forward_list有特殊的erase操作;

|删除操作|说明|
|-|-|
|c.clear()|清空容器，返回void.|
|c.pop_front()|删除首元素，返回void.|
c.pop_back()|删除尾元素，返回void.|
|c.erase(p)|删除迭代器p指向的元素，返回指向被删除元素**后面元素**的迭代器.若p是尾后迭代器,行为**未定义**|
|c.erase(b,e)|删除b,e迭代器对表示范围内的元素,返回指向最后一个被删除元素后面元素的迭代器.若e为尾后迭代器,反馈尾后迭代器.|
|-------------------------|--------------------------------------------------------------------------------------------------|
|lst.erase_after(p)|删除迭代器**p指向位置后面**的元素,返回指向被删除元素**后面元素**的迭代器.|
|lst.erase_after(b,e)|删除b,e迭代器对表示的范围内的元素,返回最后一个被删除元素**后面元素**的迭代器.|

* 某些容器不支持专门的删除首尾元素的操作,但是可以用erase实现.

```
//vector不支持pop.front()操作,但可以用erase完成相同的功能,但是性能可能很差.其他容器类型也如此.
vector<int> vec={1,2,3,4};
vec.pop_front(); //错误.vector不支持push)front().
vec.erase(vec.begin()); //正确
```

* 可以利用erase返回值连续删除元素

```
vector<int> vec={1,2,3,4,5,6,7,8,9,0};
vector<int>::iterator iter=vec.begin();
for(int i=0;i<10;i++)
    iter=vec.erase(iter); //删除第一个元素
```

## 大小和修改容量

1.size(),empty()所有容器都支持.
2.**forward_list**不支持maxsize().
3.shrink_to_fit只适用于能**随机访问的顺序容器**.如:vector,string,deque.
4.capacity()和reserve()仅适用于**vector,string**.
5.vector,string当且仅当容器元素个数大于容量时才会增加容量.

|操作|说明|
|-|-|
|c.size()|返回容器中元素个数|
|c.maxsize()|返回容器最多能容纳的元素个数|
|c.empty()|返回bool值.若容器为空,返回true;若容器非空,返回false.|
|--------------------|-----------------------------------------------------------------------------------------------------------------------------------|
|resize(n)|将容器元素个数修改为n.若n<c.size(),多余的元素删除;若n>c.size(),新增的元素值初始化.|
|resize(n,t)|将容器元素个数修改为n.若n<c.size(),多余的元素删除;若n>size(),新增的元素值为t. |
|--------------------|-----------------------------------------------------------------------------------------------------------------------------------|
|c.capacity|返回容器容量(目前能容纳的元素的个数)|
|c.reserve(n)|将容器容量修改为n.当且仅当n>c.capacity()时,c.reserve()才生效.|
|c.shrink_to_fit()|将容器容量改为现在的元素个数.调用shrink_to_fit仅仅是发送请求,并不保证一定退回多余的内存空间.|

## 容器关系运算

1.所有容器类型都支持==、!=操作
2.顺序容器都支持>、<、>=、<=操作
3.== 、!= 运算符实际标记上通过元素的== 运算符完成.
4.<、>、<=、>=实际上通过元素的<运算符完成.
5.由3,4知，当元素不支持== 或<操作时,容器类型就无法完成相应的关系运算符运算.

## string的特殊操作(待续)

# 容器配适器(待续)
* 容器配适器接受一个
# STL迭代器
## 迭代器失效
1.当对顺序容器进行改变容器元素个数的操作时,迭代器可能失效.
###vector,string
* *添加元素*：
    对一个vector,string添加元素时.若容器储存空间从新分配,则指向容器的迭代器、指针、引用全部失效;当容器存储空间没有重新分配时,则指向插入元素之前的迭代器、指针、引用仍有效，之后的失效.
* *删除元素*:
    删除元素之前的迭代器、指针、引用仍有效，之后的失效.
###list,forward_list
* 不论是添加元素还是删除元素,指向其他位置的迭代器、指针、引用仍有效,
###deque
* *添加元素*:
在首尾之外插入元素,所有的迭代器、指针、引用都失效；在首尾插入元素，**迭代器失效，指针、引用仍有效**.

* *删除元素*:
    在首尾之外删除元素,所有的迭代器、指针、引用都失效；在首尾删除元素，迭代器、指针、引用**仍有效**（**在尾部删除时，尾后迭代器失效**）.

2.当程序改变容器元素个数时,必须保证每部都更新迭代器、指针或引用.

```
//删除vector容器中的偶数元素,复制奇数元素.
//此例每步都更新了迭代器.
vector<int> vec={1,2,3,4,5,6,7,8,9};
auto iter=vec.begin();
while(iter!=vec.end()){
    if(*iter%2){
        iter=vec.insert(iter,*iter);
        iter+=iter+2;
    }
    else vec.erase(iter);
}
```
## 迭代器范围
* 一对迭代器表示的范围为左闭合右开区间.数学表示[begin,end).
## 标准库定义的特殊迭代器
* 除了每个容器的迭代器,头文件**iterator**中还定义了四种迭代器.

### 插入迭代器
* 插入迭代器类型:

|插入迭代器|说明|
|-|-|
|back_inserter(c)|调用back_inserter()产生的迭代器,自动调用push_back().总是在容器尾部插入.|
|front_inserter(c)|调用front_inserter()产生的迭代器,自动调用push_front().总是在容器头部插入.|
|inserter(c,p)|调用inserter()产生的迭代器,自动调用c.insert(p),且**总是在p前面**插入.|

* 插入迭代器支持的操作:

*插入迭代器不支持递减操作*

|操作|说明|
|-|-|
|iter=t|在iter指定的位置插入值t.|
|*iter,++iter,iter++|操作合法,但**不会对迭代器做任何事情**.|
### 流迭代器
* 流并不是类型,但是表尊库定义了可以用于这些IO类型对象的迭代器.istream_iterrator读取输入流,ostream_iterator向流输出数据.通过使用流迭代器,可以用泛型算法从流对象中读取数据或向其写入数据.
* 可以向任何定义了输入运算符的类型定义ostream_iterater,向**任何定义了输出运算符的类型**定义istream_iterator.
--------

* istream_iterator操作:

|istream_iterator操作|说明|
|-|-|
|istream_iterator<T> in(is)|in从输入流读取类型为T的值|
|istream_iterator<T> end|end为读取类型为T的istream_iterator**尾后迭代器**|
|*in|解引用in,返回从流中读取的值.|
|in->men|相当于(*in).men|
|in1==in2|若in1和in2读取相同的值,并且绑定到同一个流或都为尾后迭代器,则两者相等.|
|in1!=in2|判断标准如上.|
|++in,in++|递增运算与一般递增运算相同.|
* ostream_iterator操作:

|ostream_iterator操作|说明|
|-|-|
|ostream_iterator<T> out(os)|将类型为T的值写入到输出流os中|
|ostream_iterator<T> out(os,**c_style_string**)|out将类型为T的值写入到输出流os中,并且在每个值后面输出c_style_string.|
|out=val|用<<运算符将cal写入到out绑定的输出流中,val类型必须与T兼容.|
|*out,out++,--out|与插入迭代器相同.操作合法,但**不对迭代器做任何事情**.|

eg1:在泛型算法中使用流迭代器
```
//将vec中的值用copy算法输出到cout中,并在每个值后面添加" ";
vector<int> vec={1,2,3,4,5,6,7,8,9,10};
ostream_iterator out_iterator(cout," ");
copy(vec.cbegin(),vec.cend(),out_iterator);
```
eg2:尽管解引用和递增操作不对ostream_iterator做任何事情,但是这样写更加直观清晰.
```
vector<int> vec={1,2,3,4,5,6,7,8,9};
ostream_iterator out(cout," ");
for(auto &x:vec)
    *out_terator++=x;
```

### 反向迭代器
* 反向迭代器是从尾元素向头元素反向移动的迭代器.对于反向迭代器,递增(递减)的含义与一般迭代器相反.递增一个反向迭代器,迭代器会往头元素方向移动一个位置.
* 反向迭代器**需要递减运算符**,因此顺序容器中的forward_list不支持反向迭代器.
* 反向迭代器通过*rbegin(),rcbegin(),rend(),rcend()*成员函数获得.
* 反向迭代器表示范围仍然是左闭合右开区间.**rbegin()指向尾元素,rend()指向头元素之前的位置**.数学表示[c.rbegin(),c.rend()).
* 反向迭代器从后后往前移动,输出字符时顺序从后往前,可以用**base()成员函数**完成到**正常次序**的转换.
```
//输出line最后一个逗号后的字符;
string line("FIRST,MIDDLE,LAST");
auto iter=find(line.crbegin(),line.crend(),',');
cout<<string(iter.base(),line.cend())<<endl;
cout<<string(line.crbegin(),iter)<<endl;
-------------------------------------------------------------------------------
输出:
LAST
TSAL
-------------------------------------------------------------------------------
值得注意的是iter.base()指向的位置,iter.base()返回的普通迭代器指向iter后面的'L'.
```
* 通过反向迭代器使算法透明地向前或向后执行.

eg:
```
//使用反向迭代器使sort算法将vec中的元素从大到小排列(从后往前运行算法);
vector<int> vec ={1,2,3,4,5,6,7,8,9};
sort(vec.rbegin(),vec.rend());
```

### 移动迭代器
## 迭代器类别
根据迭代器读、写、访问元素的能力划分迭代器类别.
如下:

|迭代器类别|说明|
|-|-|
|输入迭代器|只读,不写,单遍扫描,只能递增|
|输出迭代器|只写,不读,单遍扫描,只能递增|
|前向迭代器|可读可写,多遍扫描,只能递增|
|双向迭代器|可读可写,多遍扫描,可增可减|
|随机访问迭代器|可读可写,多遍扫描,随机访问|

* 这五类迭代器之间构成了**高低层次关系**.一个高级别的迭代器支持低级别的迭代器的全部操作(高层次迭代器**兼容**低层次迭代器器),传递给一个算法使用的迭代器层次必须**不能低于**算法要求的迭代器层次.
### 输入迭代器

|输入迭代器支持的操作|说明|
|-|-|
|iter1==iter2||
|iter1!=iter2||
|iter++||
|++iter||
|*iter||
|iter->member||
### 输出迭代器
 |输出迭代器支持的操作|说明|
 |-|-|
 |iter++||
 |++iter||
 |*iter||
### 前向迭代器
* 同时支持输入、输出迭代器的全部操作,**不可递减**.
### 双向迭代器
* 支持前向迭代器并可以递减.
### 随机访问迭代器
* 支持**所有**迭代器运算

|随机迭代器支持的运算|说明|
|-|-|
|< , <= , > , >= |比较运算符|
|- ,-= ,+ ,+=|赋值和复合赋值.|
|iter2-iter1|两迭代器相减得到两迭代器之间的距离,类型为difference_type.|
|iter[n]|相当于在迭代器**前进方向**移动n个位置,然后解引用.|
# STL关联容器(待续)
# STL泛型算法
* 泛型算法定义在头文件**algorithm**和**numeric**中.
* 泛型算法**运行在迭代器上**,不依赖于容器.算法可以改变容器内元素的值,更改元素的次序,但并不执行容器的操作,**不改变容器的大小**.
* 泛型算法**依赖于元素类型**的操作,元素类型必须要定义有算法需要的运算符.泛型算法允许使用自定义的操作替代默认的运算符.
## 算法形参模式
为了便于记忆算法,标准库提供的算法大部分有着相同的形式.
|形参|意义|
|-|-|
|beg,end|beg和end表示一个迭代器范围,几乎所有算法都对一个有beg和end表示的序列进行操作.|
|beg2,end2|beg2,end2分别表示第二个序列的起始位置和结束位置.    若没有end2,则算法假定第二个序列**至少和第一个序列一样长**.   beg和beg2的类型不必匹配,但必须保证两个序列中的元素都可以执行特定操作或调用给定的可调用对象.|
|dest|dest是表示目的序列的迭代器,对于给定目的序列,算法假定目的序列**必须能保存同样多的元素**.|
|unaryPred,binaryPred|一元谓词和二元谓词,分别接受一个和两个**来自输入序列**的参数,并且谓词返回值可用做**条件类型**.|
|comp|comp是一个二元谓词,满足关联容器中对关键字序的要求.
|unaryOp,binaryOp|两个可调用对象,分别接受输入序列中一个和两个参数.|

**大多数算法满足以下四种形式之一**
* alg(beg,end,*other argvs*);
* alg(beg,end,dest,*other argvs*);
* alg(beg,end,beg2,*other argvs*);
* alg(beg,end,beg2,end2,*other argvs*);

## 算法命名规范
**使用重载**
* 某些算法有重载形式.
>> unique(beg,end);      //默认使用==进行比较
>>unique(beg,end,comp)   //使用comp进行比较

**_if版本**
* 某些函数两个版本接受相同数目的参数,为了避免重载歧义,使用_if版本.
>>find(beg,end,val);
find_if(beg,end,unaryPred);

**copy版本**
* 某些算法提供了copy版本,并不改变输入序列的值,而将算法运行后的结果输出到目的序列中.
>>replace(beg,end,old_value,new_value);
replace_copy(beg,end,dest,old_value,new_value);

## 特定容器算法(部分待续)
* list和forward_list由于采用链式结构,定义了某些特有的算法.
* 对与list和forward_list应当优先使用成员函数版本的算法.
## 定制操作

* 可调用对象:如果个对象或表达式可以使用调用运算符,那么这个对象或表达式就是可调用对象.
* 谓词:一个返回结果能用做**条件**的值,并且接受**一个或两个**参数的可调用对象.
* 用自定义的操作替换算法默认的运算符,只需要在参数列表中传递一个谓词或者可调用对象.
### lambada表达式

有时需要向谓词传递大于两个参数,但是参数个数大于谓词能接受的最大个数,使用lambda表达式可以解决这个问题.
#### 基本形式

**[capture_lsit] (parameter_lsit) ->return_type {function_body}**
* parameter_list和return_type可以省略
* return_type必须使用尾置返回类型.
* capture_lsit和function_body不可省略.
#### 表达式创建

* 当定义一个lambda时,编译器生成一个与lambda对应的新的、未命名的类类型.
* 当向函数传递lambda时,同时定义了一个新类类型并由该类创建了一个对象.
* 当使用auto定义一个用lambda初始化的变量时,定义了一个从lambda生成的类型的对象.
* 从lambda生成的类包含的数据成员是捕获列表中的成员.当lambda对象被创建时,lambda的数据成员被初始化.
#### 捕获列表
* lambda捕获方式分为**值捕获**和**引用捕获**
* lambda可以直接使用所在函数之前的变量和lambda之前的static变量.变量捕获只是指捕获lambda之前的非static变量.

|捕获列表|说明|
|-|-|
|[]|空捕获列表.lambda不捕获变量.|
|[names]|lambda捕获列表中的变量.名字前加&的引用捕获,否则值捕获.|
|[&]|默认引用捕获.|
|[=]|默认值捕获|
|[&,identifier_list]|identifier_list中的变量必须用**值捕获**方式,其他变量**默认引用捕获**.|
|[=,identifier_list]|identifier_list中的变量不许用**引用捕获**方式,其他变量**默认值捕获**|
##### 值捕获

* 值捕获的前提是捕获的变量**可以拷贝**.
```'
void func(){
    size_t v1=42;
    auto f=[v1] {return v1;}; //捕获v1;
    v1=0;   //令v1为0
    auto j=f(); //j等于42.f保存了创建它时v1的拷贝.修改v1对lambda对象f没影响.
}
```
##### 引用捕获

* 捕获引用、指针、迭代器时，必须保证在lambda**执行时变量存在**.
* 当lambda返回引用时,必须保证引用的对象存在,并且**不能返回指向局部变量的引用**.
```
void func(){
    size_t v1=42;
    auto f=[&v1] {return v1;}; //捕获v1;
    v1=0;   //令v1为0
    auto j=f(); //j为0.f保存v1的引用.
}
```
##### 隐式捕获

* 不仅可以显式地指定值捕获和引用捕获，也可以隐式地指定捕获类型.此时默认以隐式捕获确定的方式捕获lambda之前的所有变量.
```
//words是string类型.
wc=find_if(words.begin(),words.end(),
            [=]A(const strint &s)  //sz隐式捕获,值捕获类型.
                { return s.size()>=sz;});
```

```
//words是string类型;os显示捕获,引用捕获类型;c隐式捕获,值捕获类型.
for_each(words.begin(),words.end(),
        [=,&os] (const string &s) { os<<s<<c;});
```
##### 可变lambda

* 对于值捕获的变量,lambda不能改变其值.此时可以在lambda形参列表后面加上mutable来改变值捕获变量的值.
```
void func(){
    size_t v1=10;
    auto f=[=] {return ++v1;}; //错误.lambda不改变值捕获的变量的值.
    auto f=[=] () mutable {return ++v1;}; //正确.可变lambda.可以改变值捕获的变量的值.
}
```
#### 形参列表
* lambda形参列表和一般函数的形参列表相同,但**不允许使用默认实参**.
#### 返回类型
* 当lambda表达式的function_body中有**除return以外的语句**,并且**未指定return_type**时,lambda自动返回**void**.
```
//利用transform算法将vector<int>容器中的值转换为绝对值;
vector<int> vec={-1,2,-3,4,-5,6,-7,8,-9};
//正确,lambda函数体中只有return语句,编译器自动推断出返回类型为int.
transform(vec.begin(),vec.end(),
           [](int i) {return i<0?-i:i});
//错误.lambda函数体中有除return之外的语句,自动推断出返回void,与实际情况不同.
transform(vec.begin(),bec.end(),
            [](int i) {if(i<0) return -1; else return i;})
```
#### 函数体
* lambda函数体与一般函数的函数体相同.
### 函数配适器:bind函数

* lambda只适用于一两个地方使用的简单操作.当某个操作需要反复使用或者较为复杂时,使用bind函数解决.
* bind函数和bind函数所需的cref、ref函数都定义在**functional**头文件中.
#### 一般形式:

**auto newCallable=bind(callable,args_list);**
>>注释:
>>1.newCallable是一个可调用对象.
>>2.args_lsit是一个逗号分隔的参数列表,**对应着callable的形参列表**.
>>3.args_list中可能有形如_n的占位符.这些占位符表示newCallable中相应位置的参数._1表示newCallable的第一个参数,_2表示newCallable的第二个参数,以此类推.
>>4.当调用newCallable时,newCallable调用callable,并将占位符表示的参数传递给callable.

```
//check_sz定义:
//bool check_sz(const stirng &s,string::size_type sz){
//    return s.size()>s=sz;
//}
string s="Hello";
auto check6=bind(check_sz,_1,6); //定义newCallable,newCallable的第一个参数传递给callable;
//相当于:
//auto b1=check(s,6);
bool b1=check6(s);
```
#### 占位符
* 占位符_n都定义在命名空间**std::placeholders**中.使用占位符必须提供命名空间声明.
```
//使用_1,_2;
using namespace std::namespaceholders::_1;
using namespace std::namespaceholders::_2;
//可以以直接声明命名空间std::namespaceholders;
using namesapce std::namespacehodlers;
```
#### bind参数
* bind函数配适器不仅可以避开算法对谓词参数个数的限制,还可以改变可调用对象中的参数的次序
```
//f(a,b,c,d,e,f)是一个有5个参数的函数;
auto g=bind(f,a,b,_2,d,e,_1_);
//bind调用g(X,Y)会被映射为:
//f(a,,b,Y,d,e,X);
```
* 利用bind重排参数次序
```
//words是string类型;isShorter函数当一个参数长度小于第二个时返回真,否则返回假;
//单词长度从小到大;
sort(words.begin(),words.end(),isShorter);
//单词长度从大到小;
sort(words.begin(),words.end(),bind(isShorter,_2,_1));
```
* **绑定引用参数**

bind那些不是占位符的参数被**拷贝**到bind返回的newCallable中,但是某些参数无法拷贝,此时使用标准库函数**ref/cref**.ref返回一个对象,包含给定的引用,此对象可以拷贝.cref返回保存const引用的对象.
```
//ostream &print(ostream &os,const string &s,char c){
//    return os<<s<<c;
//}
for_each(words.cbegin(),words.cend(),bind(print,ref(os),_1,_2); //由于os是ostream类型对象,不可拷贝,因此使用标准库函数ref;
```






















































