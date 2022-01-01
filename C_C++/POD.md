# Overview

C++ class aggregate data and operations(function) into single class. The most trickle problem arose from the design is that the layout of class  perhaps differentiates from ordinary old struct in C. If some type is  not compatible with C struct type, it can't interact with C program  directly. Moreover, Many class in C++ is not memory-continuous, it can't be transfered between Internet software.

To solve this problem, C++ introduce the concept of *POD(plain old data)*. POD is a type which is compatible with C program. That is to say, The  type is memory-continuous(stored continuously in memory) and  C-layout-compatible(the layout of class is same with C struct/union).

After C++11, C++ has not emphasized any more. POD is defined as the type satisfies *standard layout* and *trivial type*. Trivial type is memory-continues and bitwise-copyable(can be copy using `memcpy()` and restored correctly), standard layout type is C-layout-compatible.

# Standard Layout

Standard layout is the type satisfies the following restrictions:

- has **no virtual** functions and no virtual base classes.
- has the **same** access control (private, protected, public) for all its **no-static** data members.
- only **one** class can has non-static member in inheritance hierarchy. That is to  say, either has no non-static data members in the most derived class and at most one base class with non-static data members, or has no base  classes with non-static data members.
- **any sub-objects** (member and base class) are standard layout type.
- has no base classes(include base of base ...) of the same type as its **first** non-static data member.

If class has virtual function or virtual base class, there is *vtpr*(*virtual table pointer*) in class bringing about non-compatibility with C.

For historical reasons, compiler would do some extra implicit things which modify layout of type.

It is worth noting that all restrictions only act on non-static data member.

 General say, a type is whether standard type is determined by the non-static member in class hierarchy.

# Trivial Type

Trivial type is the type satisfies the following conditions:

- uses the implicitly defined(or default()) default, copy and move constructors, copy and move assignments, and destructor.
- has no virtual sub-objects.
- **has no non-static data members with brace- or equal- initializers**.
- its base class and non-static data members (if any) are themselves also trivial types.

What is trivial? The answerer *AnT* in stackoverflow gives the wonderful answer as following:

​	In simple words a "trivial" special member function literally means a  member function that does its job in a very straightforward manner. The  "straightforward manner" means different thing for different kinds of  special member functions.

​	For a default constructor and destructor being "trivial" means  literally "do nothing at all". For copy-constructor and copy-assignment  operator, being "trivial" means literally "be equivalent to simple raw  memory copying" (like copy with memcpy).

​	If you define a constructor yourself, it is considered non-trivial,  even if it doesn't do anything, so a trivial constructor must be  implicitly defined by the compiler.

​	In order for a special member function to satisfy the above  requirements, the class must have a very simplistic structure, it must  not require any hidden initializations when an object is being created  or destroyed, or any hidden additional internal manipulations when it is being copied.

​	For example, if class has virtual functions, it will require some extra hidden initializations when objects of this class are being created  (initialize virtual method table and such), so the constructor for this  class will not qualify as trivial.

​	For another example, if a class has virtual base classes, then each  object of this class might contain hidden pointers that point to other  parts of the very same object. Such a self-referential object cannot be  copied by a simple raw memory copy routine (like memcpy). Extra  manipulations will be necessary to properly re-initialize the hidden  pointers in the copy. For this reason the copy constructor and  copy-assignment operator for this class will not qualify as trivial.

​	For obvious reasons, this requirement is recursive: all sub-objects of  the class (bases and non-static members) must also have trivial  constructors.

Significantly, trivial is not a synonym for **trivially copyable**(bitwise copyable). One of the  conditions of POD is - *has no non-static data members with brace- or equal- initializers*. This resection is to guarantee the safety of *static initialization*. Obviously, whether a type can be bitwise-copyable has nothing to do  with the initialization. Hence, despite a type don't satisfies the  restriction of *has no non-static data members with brace- or equal- initializers*, it also can be bitwise-copyable. This feature of type is also called *trivial copyable*.

# POD

As is stated above, POD is a type satisfies standard-layout and trivial simultaneously.

C++ provide various facilities(template struct in header _type_traits_) to determine the type of class. For instance, `std::is_trivial`, `std::is_standard_layout`, `std::is_trivial_copyable`,`std::trivial_default_constructable` and so on.

# Example

```
#include <iostream>
#include <type_traits>


class A
{
    int a_val;
};

struct B : public A
{
    int b_al;
};

class virt
{
    virtual void virtual_function() = 0;
};

class C : public A
{
public:
    A a;
    int i;
    int j;
    int arr[10] = {0};
    static void foo1();
private:
    static void foo2();
};

int main()
{
    std::cout << "class C is standard layout type" << " " << std::is_standard_layout<C>::value << std::endl;
    std::cout << "class C is trivial type" << " " << std::is_trivial<C>::value << std::endl;
    std::cout << "class C is POD type" << " " << std::is_pod<C>::value << std::endl;
    std::cout << "class C is trivially copyable type" << " " << std::is_trivially_copyable<C>::value << std::endl;
    std::cout << "class C is trivially default constructable type" << " " << std::is_trivially_default_constructible<C>::value << std::endl;
    return 0;
}
----------------------------------------------------------------------------------------------------
class C is standard layout type 0
class C is trivial type 0
class C is POD type 0
class C is trivially copyable type 1
class C is trivially default constructable type 0
```

# Resources

*AnT*'s wonderful answer: https://stackoverflow.com/questions/3899223/what-is-a-non-trivial-constructor-in-c

知乎问题“什么是POD”: [https://www.zhihu.com/search?type=content&q=C%2B%2B%20pod](https://www.zhihu.com/search?type=content&q=C%2B%2B pod)

果冻虾仁关于虚表的回答: https://www.zhihu.com/question/389546003/answer/1194780618

果冻虾仁关于虚表中 type_info 的回答: https://www.zhihu.com/question/29251261/answer/1297439131