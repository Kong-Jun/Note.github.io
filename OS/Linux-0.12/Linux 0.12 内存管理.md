问题

- [ ] `share_page()`

- [ ] 为什么内核的线性地址是线性映射

- [ ] `free_bucket_desc`指向一页，存放描述符静态链表。当该页的描述符用尽后，`free_bucket_desc`为`NULL`，`init_bucket_desc()`再分配一页存储描述符静态链表，前面被使用的描述符全部被释放，但是这些描述符所在的页却不会被释放（也没有指针指向它们，想释放也没办法），导致内存泄漏。

- [ ] `malloc()`是内核分配函数，但是因为

  

  

# 概述

# 初始化

kernel/mm.h 中定义了一些物理内存管理相关的宏。

```c

```



# 物理内存

`get_free_page()`

`free_page()`



# 虚拟内存

`free_page_tables()`

`put_page()`	

`get_empty_page()`

`put_dirty_page()`





# 页面共享

`copy_page_tables()`

`try_to_share()`

`share_page()`

# 写保护和缺页异常

`un_wp_page()`

`do_wp_page()`



# 内核内存分配函数`malloc()`

## 算法

**分裂和合并**

**空间不足时的处理**

**限制**

**可重入性**

## 并发问题的处理

- 分配时：关闭中断
- 释放时：反复判断

## 指针技巧

