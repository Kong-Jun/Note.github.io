# 在无序关联容器中使用自定义的类型

无序容器的声明如下（以`unoerdered_map`为例）：

```c++
template<
    class Key,
    class T,
    class Hash = std::hash<Key>,
    class KeyEqual = std::equal_to<Key>,
    class Allocator = std::allocator< std::pair<const Key, T> >
> class unordered_map;
```

无序容器通过哈希来组织元素。STL 为所有内置类型和标准库类型定义了`operator==`和哈希函数对象`hash<>`，所以所有的内置和标准库类型都可以直接使用无序关联容器。如果需要使用自定义类型，就必须为无须容器提供：

- 相等运算符
- 哈希函数对象

通过无序容器的声明可以看出，通常有两种方式将自定义类型与 STL 结合在一起：

1.  **特例化无序容器**

   例如，我们定义了类`people`，并为它实现了函数对象`people_equal`（用于判断两个类是否相等）和`people_hash`（对类进行哈希），可以这样特例化无序容器

   ```c++
   std::unordred_map<people, int, people_equal, people_has> people_int_map;
   ```

   但是，这样非常繁琐，可读性也很差。

2. **特例化`std::hash`和`std::equal_to`**

   为了特例化`std::hash`，需要打开命令空间`std`，定义类型`argument_type`和`result_type`，并重载`operator()`即可。

   为了特例化`std::equal_to`，只需要为自定义的类提供`operator==`（`std::equal_to`内部使用`==`运算符）。

   这种方法不需要写很长的类型声明，也不需要专门定义函数对象，无序容器自动的获得了处理自定义类型的能力，因此这种方式显得更加优雅。

以下是使用`std::unordered_map`的实例，其中还未自定义类型`custommer`重载了`operator<<`，以便可以直接使用`std::cout`输出它。

```c++
#include <algorithm>
#include <iostream>
#include <iterator>
#include <ostream>
#include <string_view>
#include <unordered_map>
#include <string>

class custommer {
public:
    std::string id;
    std::string name;
    custommer(std::string_view _id, std::string_view _name)
        : id(_id), name(_name) {}
};

bool operator==(const custommer &lhs, const custommer &rhs)
{
    return lhs.id == rhs.id && lhs.name == rhs.name;
}

namespace std
{
template <> struct hash<custommer> {
  using argument_type = custommer;
  using result_type = std::size_t;
  result_type operator()(const argument_type &arg) const {
    return hash<std::string>{}(arg.id) + hash<std::string>{}(arg.name);
  }
};
}

namespace std
{
    std::ostream& operator<<(std::ostream &os, const custommer &item)
    {
        os << "{ " << item.name << ", "<< item.id << " }";
        return os;
    }
}

int main()
{
  custommer user1("KongJun", "320180938541");
  custommer user2("ZhaoHui", "320180938533");
  std::unordered_map<custommer, std::size_t> custommer_number_map{
      {user1, 100},
      {user2, 200}
  };

  std::for_each(std::cbegin(custommer_number_map), std::cend(custommer_number_map),
          [] (const auto &iter) {
                std::cout << iter.first << "  " << iter.second << std::endl;
          });
  return 0;
}


----------------------------------------------------------------------------
{ 320180938533, ZhaoHui }  200
{ 320180938541, KongJun }  100


```

在为自定义类型拓展无序容器时一定要注意：**千万不要弄错要修改的函数、函数模板、类模板的声明**。否则，可能会遇到几万行的模板报错。

这里要注意`std::hash`的`operator()`是一个 const 成员函数，如果没有将其声明为 const 的，编译时会报错：无法匹配到`operator ()`。



# 迭代器



