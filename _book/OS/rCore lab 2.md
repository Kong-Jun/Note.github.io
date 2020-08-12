# rCore lab 2

```rust
//! 自己实现的基于线段树的分配器[`SegmentTreeAllocator]

use super::Allocator;
use alloc::vec::Vec;
use core::alloc::Layout;
use core::mem;
use core::option::Option::{Some, None};


pub struct SegmentTreeAllocator {
    vec: Vec<usize>,
    capacity: usize,
}

// 获取左节点下标
#[inline(always)]
fn left(node: usize) -> usize {
    2 * node + 1
}

// 获取右节点下标
#[inline(always)]
fn right(node: usize) -> usize {
    2 * node + 2
}

// 计算区间中
#[inline(always)]
fn mid(start: usize, end: usize) -> usize {
    (start + end) / 2
}

fn build_segment_tree(vec: &mut Vec<usize>, root: usize, start: usize, end: usize) {
    assert!(start <= end);

    if start == end {
        vec[root] = 1;
    } else {
        build_segment_tree(vec, left(root), start, mid(start, end));
        build_segment_tree(vec, right(root), mid(start, end) + 1, end);

        vec[root] = vec[left(root)] + vec[right(root)];
    } 

}

fn get_offset(vec: &Vec<usize>, mut root: usize, mut start: usize, mut end: usize) -> usize {
    assert!(start <= end);
    loop {
        if start == end {
            break;
        }

        if vec[left(root)] >= 1 {
            root = left(root);
            end = mid(start, end);
        } else {
            root = right(root);
            start = mid(start, end) + 1;
        }
    }

    start
}

fn update_segment_tree(vec: &mut Vec<usize>, root: usize, start: usize, end: usize, offset: usize, val: usize) {
    assert!(start <= end);
    if start == end {
        assert!(vec[root] == 1 || vec[root] == 0);
        vec[root] = val;
    }
    else {
        if offset <= mid(start, end) {
            update_segment_tree(vec, left(root), start, mid(start, end), offset, val);
        }
        else {
            update_segment_tree(vec, right(root), mid(start, end) + 1, end, offset, val);
        }
            vec[root] = vec[left(root)] + vec[right(root)];
    }

}
impl Allocator for SegmentTreeAllocator {
    fn new(capacity: usize) -> Self {
        // 直接 push 上万个元素影响性能。
        // 我想在堆上分配数组，然后通过 unsafe 方法将其转化为 Vec，怎么做？？
        // 在文档中看到 Vec<T> 实现了 From<Box<[T]>> Trait，尝试使用 Box 在堆上分配 数组，再利用 From Trait 转化为 Vec。
        // 但是这样行不通，数组的大小在运行时才能确定，而 Rust 不支持动态数组，我不知道怎样像 C 那样利用 malloc() 分配一段内存作为数组。
        // rCore 支持的最大内存是已知的（128M)，所以用 Box 分配一个大小足够记录所有页的数组，再将其转化为 Vec。但是在编程的过程中发现用 Box 分配数组会导致分配器分配失败，最终使用不安全特性手动分配内存解决了这个问题。
   
		let layout:Layout = unsafe {
			Layout::from_size_align_unchecked(3 * capacity * mem::size_of::<usize>(), mem::size_of::<usize>())
		};

		// 分配内存
		let ptr: *mut usize = unsafe {
			alloc::alloc::alloc(layout) as *mut usize
		}; 
		let mut vec = unsafe {
			Vec::from_raw_parts(ptr, 3 * capacity, 3 * capacity)
		};
        
        build_segment_tree(&mut vec, 0, 0, capacity - 1);
        Self {
            vec: vec,
            capacity: capacity,
        }
    }

    fn alloc(&mut self) -> Option<usize> {
        let result;
        result = if self.capacity == 0 || self.vec[0] == 0 {
            None
        } else {
            Some(get_offset(&self.vec, 0, 0, self.capacity -1))
        };
        if let Some(offset) = result {
            update_segment_tree(&mut self.vec, 0, 0, self.capacity - 1, offset, 0);
        }
        result
    }

    fn dealloc(&mut self, index: usize) {
        update_segment_tree(&mut self.vec, 0, 0, self.capacity - 1, index, 1);
    }
}


```



# lab-3

`/os/src/memory/address.rs`中封装了物理页号、虚拟页号、物理地址、虚拟地址，并定义了一些操作。其中的操作主要分为三类：

1. 虚实转换
2. 
3. 页号和地址的转换
4. 和`usize`类型的交互

**虚实转换**

- 虚实地址和虚实页号之间的映射（相互转换）
- 指针转换为虚拟地址

**解引用操作**

- 虚拟地址转换为`&mut T`
- 物理地址（内核）转换为`&mut T`
- 虚拟页号转换为`[u8; PAGE_SIZE]`
- 物理页号转换为`[u8; PAGE_SIZE]`

**页号和地址的转换：**

- 从页号到地址：直接位移
- 从地址到页号：直接位移
- `floor()`：从地址到页号，向下取整
- `ceil()`：从地址到页号，向上取整

**与`usize`类型的交互**：

- 算术运算：直接修改页号/地址的值
- 类型转换：页号/地址和`usize`整数之间的转换

除此之外，还定义了从虚拟地址获取 VPN 的函数`levels()`和判断地址/页号是否有效的函数`valid()`。



## 物理内存分配器和建立映射的作用

lab-3 完成了从物理内存到虚拟内存的转换，所有的代码、数据都在虚拟内存的机制下访问。将一段虚拟内存映射至物理内存就意味着被映射的物理内存已经被使用，这段物理内存不能再被用于其他用途。为了保证这段物理内存不再被二次分配，操作系统必须知道哪些物理内存已经被分配。

rCore 内核被加载到了物理地址 `0x80200000`，内核占用的内存不在物理内存管理器的管理范围内，物理内存管理器对内核所在的内存区域毫不知情，只管理内核之上（`kernel_end`之后）的内存。物理内存管理的范围直接影响了完成虚拟内存到物理内存映射的关系。

对于操作系统内核，内核在开启虚拟内存之前就在物理内存中，而且地址是确定的，适合使用线性偏移的方式进行映射，将物理地址加上一个固定的偏移作为虚拟地址。在建立虚拟地址到物理地址的映射的过程中，内核所在的物理地址没有任何变化，也没有发生数据的复制，物理内存分配器的作用仅仅是分配物理页作为页表。

对于应用程序，使用的内存都在物理内存分配器的管理下，一半通过`Framed`方式进行虚拟地址映射，因为程序的指令、数据都不在物理内存中，需要分配多段物理内存并将可执行文件中的指令、数据分配到其中，所以建立映射的过程中发生了数据的复制，内存分配器不仅要分配内存作为页表，也要分配内存给应用程序。



# lab-4



os/src/interrupt/mod.rs 中 `wait_for_interrupt()`函数为什么先禁用时钟中断再开启全局中断，使用汇编指令 `wfi`，再关闭全局中断，开启时钟中断？

> 先禁用时钟中断再开启全局中断，时钟中断外的所有中断都可以正常发生，所以程序会等待，直到有除了时钟中断之外的中断发生。
>
> 关闭全局中断，开启时钟中断后，所有中断都不能发生，提前开启时钟中断应该只是为了避免下次打开全局中断时忘记打开时钟中断。

