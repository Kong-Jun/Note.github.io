[TOC]

## Overview

**STL**(Standard Template Library) is the heart of the C++ standard library,which provides solutions to managing collections of data with modern and efficient algorithms.STL is not only a generic library of C++,but also a language part of it for giving C++ a new level of abstraction

STL is a masterpiece of generic programming. It is based on the cooperation of various well-structured components,key of which a containers,iterators and algorithms.

**Container**

* Container like a basket which contain sorts of data elements.

**Iterator**

* Iterator is an abstraction of position of data elements in container.It is like a compass indicating the positon of data elements.It offers a small but common *iterface* for any arbitrary container type.We can use iterator directly without any awareness of lower implementation of container.

**Algorithm**

* Algorithms are encapsulation of some clasical algorithms such as seaching,sorting and etc.Algorithms also give us flexibility 某种程度 by allowing programmer supply certain auxiliary(辅助的) functions for algorithm to call.

This diagram illustrate the relations among containers,iterators and algorithms.

![STL Components](https://github.com/Kong-Jun/note-picture/blob/master/DeepinScreenshot_select-area_20191119201309.png)

Of course,STL consists of more generic components,such as *adaptor*, *function object* and others.

## Containers

We can categorize STL containers in three types.

**Sequential containers**

In this type container,data elements are in linear order.The order of elements depend on the relative order and place of insertion.

**Associative containers**

In this type containers, data elements are key(set and multi_set) or key/value pair(map or multi_map) and automatically sorted by predefined *function objec* called "less<>"(template less<> call operator<). The order of elements is only depend on the relative size. The time and place of insertion doesn't matter.

**Unordered containers**

In the authoritative document,unordered container is a part of associative container instead of counterpart of it. But here, we think it is  counterpart of associative container.

In this  container,elements are out of order. In the lifetime of container, the relative order is changeable when insert or remove elements. The only important thing is whether a element is in the container.

### internal structure

C++ standard has not prescribe the specific implementation of STL.However, owing to the complexity prescribed by standard, there is no room for variation.Thus,almost all STL are implemented in the same data structure.

Understanding the usual internal structure of STL is greatly helpful for us to grok the benefits and pitfalls of STL. 

The usual internal structure as follows:

![Containters internal structure](https://github.com/Kong-Jun/note-picture/blob/master/DeepinScreenshot_select-area_20191119203030.png)

In the diagram,we can see it:

1. Sequence containers are implemented as fixed array, dynamic array, doubly queue, doubly linked list and singly linked list.
2. Associative containers are implementd as binary tree(serching tree).
3. Unordered containers are implemented as hash table.

The internal data structure of containers directly decide the features and methods it provide.


#### array
Array is distinct from other containers in STL. It is designed as substitute of C-style array. It does not  provide inserting,emplacing,removing interface or another interface that can change the size of array. Resulting  from this property, we ignore it when introducing detail container interface by default.

In modern C++. It is not necessary to use C-style array.
#### vector
Vector is implemented as dynamic array. Inserting or removing elements on the rear is very fast, but in other places perhaps case performance penalty.
#### deque
Deque is implemented as two-side queue. It can grow fast toward two side. Inserting and removing in front and rear is fast,but in other places  will case performance penalty.
#### list
List is implemented as doubly linked list.Inserting and removing in any place is fast benefiting by the linked storage.
#### forward_list
Forward_list is implemented as singly linked list. Inserting and removing in any place is fast. Forward linked is a restricted list.It only can iterate over element forward resulting from its singly linked structure. The singly linked structure make it inconvenient to traverse elements. Thus,in practice,we also choose  list instead of forward_list.

Because forward_list is a special list,many interface is indentical (完全一样,拼写错误) with list.
#### string
It is a container of char type data. String can be considered as a part of STL, despite that it's style is non-compatible with other parts of STL. It can interact with iterator, algorithm and other STL components.
### trade-off

One of the principles of STL is that only provide the interface that has good performance(constant and logarithmic complexity). 

For this reason, vector and string does not provide operations refer to front.Forward_list does not provide operations refer to back.



## Iterators
### Overview
### Categories

Input iterator:Read/Process
Output iterator:Write/Assign
## Iterator Adaptor
Insert iterator
Insert iterator allow algorithm operate in insert mode rather than in overwrite mode.

## Algorithms

### Ranges
# ensuring the correction of range is the responsibility of programmer.(empehsize)
![multiple ranges](https://github.com/Kong-Jun/note-picture/blob/master/DeepinScreenshot_select-area_20191120213644.png)

### "Removing" algorithms
This program represents the side-effect of STL's separation of data and operation.

```cpp
#include<algorithm>
#include<iterator>
#include<list>
#include<iostream>
using namespace std;
int main()
{
  list<int>coll;
  for(int i=1;i<=6;++i){
    coll.push_front(i);
    coll.push_back(i);
  }
  cout<< "pre:";
  copy(coll.cbegin(),coll.cend(),ostream_iterator<int>(cout," "));
  cout<< endl;
  remove(coll.begin(),coll.end(),3);
  cout<< "post:";
  copy(coll.cbegin(),coll.cend(),ostream_iterator<int>(cout," "));
  cout<< endl;
  return 0;
}
---------------------------------
Outcome:
---------------------------------
pre:6 5 4 3 2 1 1 2 3 4 5 6 
post:6 5 4 2 1 1 2 4 5 6 5 6 
![How remove work](https://github.com/Kong-Jun/note-picture/blob/master/DeepinScreenshot_select-area_20191120220759.png)
