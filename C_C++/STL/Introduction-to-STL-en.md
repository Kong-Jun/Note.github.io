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
List is implemented as doubly linked list.Inserting and removing in any place is fast benefited by the linked storage.
#### forward_list
Forward_list is implemented as singly linked list. Inserting and removing in any place is fast. Forward linked is a restricted list.It only can iterate over element forward resulting from its singly linked structure. The singly linked structure make it inconvenient to traverse elements. Thus,in practice,we also choose  list instead of forward_list.

Because forward_list is a special list,many interface is indentical (完全一样,拼写错误) with list.
#### string
It is a container of char type data. String can be considered as a part of STL, despite that it's style is non-compatible with other parts of STL. It can interact with iterator, algorithm and other STL components.
### trade-off

One of the principles of STL is that only provide the interface that has good performance(constant and logarithmic complexity). 

For this reason, vector and string does not provide operations refer to front(push_front(),emplace_front(),pop_front()).

Forward_list does not provide operations refer to back(push_back(), emplace_back(), pop_back()). Forward_list does not provide size() method to conserve memory.

In the associative container, container maintain an specific order. In the unordered container, container is out of order. To avoid break the internal order, associative container and unordered container forbid changing elements' key and inserting elements in a user-decided place. User only can choose whether inserting elements and inserting what elements. Container will insert the element in correct place in their way.

Not only that, this feature results in limits of algorithms that work on associative and unordered container. For instance, we can't use most modifying algorithms on them.

Despite of inserting elements in a user-decided place is forbidden, containers give a user a guarantee about the relative order of inserted elements.

In associative container, the sorting criterion is guaranteed as table, the inserted element would be adjacent and newly inserted elements are behind the old elements.

In unordered container, the elements of same value would be adjacent. 

Only understanding the features and trade-off of containers and algorithms, can we using STL flexibly and get away pitfalls and abusing.
## Iterators
### Overview
Iterator is a pure abstraction of position of element in container. It give user the ability to write user-defined iterators of user-defined container to interact with STL.  It is the platform on which algorithms run.

While making generic algorithms possible, iterators also bring about some troubles. It is penalty of the separation of data and operations.
### Categories
Classified by iterating behavior, iterators fall into three categories.

 **forward iterator**

 Forward iterator is only able to iterate over elements forward, it has no ability to go back.Forward_list,all unordered associative provide this iterator.

 **bidirectional iterator**
 
 Bidirectional iterator is able to iterate over elements in tow directions: forward and backward. List, all associative container provide bidirectional iterator.

 **random-access iterator**

 Random-access iterator can performance random access. In particular, they provide for iterator arithmetic (in acorrdance with ordinary pointer arithmetic).

 It is worth noting that there is a hierarchy of iterators. High-level iterator has all ability of low-level iterator. The hierarchy of this three categories as follow.

 Random-access iterator>>bidirectional iterator>>forward iterator.

Classified by read/write behavior, iterators fall into two categories.

**Input iterator:Read/Process**

Input iterator is able to read/process elements of container. Such as istream_iter that is introduced in later session.

**Output iterator:Write/Assign**

Output iterator is able to write/assign elements of container. Such as ostream_iter that is also introduced in latter session.

## Iterator Adaptor
Adaptor assign more unordinary abilities to iterator.

Insert iterator allow algorithm operate in insert mode rather than in overwrite mode.

**Insert iterator**

Ordinary just a abstraction of position. But insert iterator has the ability to *create* a room in container and *insert* element there.

Insert iterator can be subdivided into three category:

1. **back_inserter**(*container*): call push_back() interface of container.
2. **front_inserter**(*container*): call push_front() interface of container.
3. **inserter**(*container*,*pos_iter*): call insert(*pos_iter*) interface of container.

Containers are able to use insert iterator only when they provide for the interface insert iterator calls.

Generally, insert iterators are used by sequential containers, but associate container also can use *inserter* which *advise* a place to insert rather than *demand* or *choose* a place like sequential container.

###Stream iterator
Stream iterators give algorithms ability to interact with IO stream as a sequential container.

Consider this code segment:

```cpp
//using stream iterator and copy algorithm initialize vec by values from cin.
#include<iostream>
#include<vector>
#include<algorithm>
#include<iterator>
using namespace std
int main(){
  istream_iterator<int> in(cin),end;
  vector<int> vec;
  copy(in,end,back_inserter(vec));
  return 0;
}
```
Stream iterator fall into istream_iterator and ostream_iterator. They are both template class. istream_iterator is input iterator and ostream_iterator is output iterator.

### reverse iterator
Reverse iterator iterate over element in the reverse direction of normal iterator. It's operator++ is like normal iterator's operator--.

### move iterator
Move iterator use *move semantic*, dereference a move iterator gain a *right value reference* rather than normal iterator's *left value reference*.

Move iterator is dangerous, use it only when you can ensure algorithm can receive it safely.

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

```
![How remove work](https://github.com/Kong-Jun/note-picture/blob/master/DeepinScreenshot_select-area_20191120220759.png)
