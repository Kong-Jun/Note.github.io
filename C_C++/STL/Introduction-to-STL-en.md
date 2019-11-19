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



We can categorize STL containers in three types:
1. **Sequential containers**

In this type container,data elements are in linear order.The order of elements depend on the relative order and place of insertion.

2. **Associative containers**

In this type containers, data elements are key(set and multi_set) or key/value pair(map or multi_map) and automatically sorted by predefined *function objec* called "less<>"(template less<> call operator<). The order of elements is only depend on the relative size. The time and place of insertion doesn't matter.

3. **Unordered containers**

In the authoritative document,unordered container is a part of associative container instead of counterpart of it. But here, we think it is  counterpart of associative container.

In the container,elements are out of order. In the lifetime of container, the relative order is changeable when insert or remove elements. The only important thing is whether a element is in the container.

### internal structure

C++ standard has not prescribe the specific implementation of STL.However, owing to the complexity prescribed by standard, there is no room for variation.Thus,almost all STL are implemented in the same data structure.

Understanding the usual internal structure of STL is greatly helpful for us to grok the benefits and trade-offs of STL. Therefore,let us 看看 the internal of STL.

![Containters internal structure](https://github.com/Kong-Jun/note-picture/blob/master/DeepinScreenshot_select-area_20191119203030.png)

In the diagram,we can see it:

1. Sequenc containers are implemented as fixed array, dynamic array, doubly queue, doubly linked list and singly linked list.
2. Associative containers are implementd as binary tree(serching tree).
3. Unordered containers are implemented as hash table.

The internal data structure of containers directly decide the features and methods it provide.

#### array

