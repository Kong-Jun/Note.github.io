# lab-1

lab-1 的代码还比较简单，仅仅是实现了时钟中断和断点。lab-1 中开启了 sstatus 寄存器中的 SIE 域，允许内核被中断，但是仅开启了时钟中断和外中断。

## 启动部分

程序的入口点在 os/src/entry.asm 中，这个文件设置了程序最开始的页目录和堆栈，堆栈共 16 KB，位于 .bss 段中。

页目录的内容暂时还不清楚是做什么的，之后再看?

> 这部分的内容在 lab 3 中，设置了一个临时映射，使开启分页和跳转 `rust_main()`之后可以正常运行。



## 中断



rCore 中断发生的过程如下：

1. 发生中断，PC 被设置到`__interrupt()`，该函数作为中断处理的统一入口，将所有通用寄存器、 sstatus 寄存器和 sepc 寄存器压栈，这个结构就是 `Context`，并将 stval 寄存器和 scause 寄存器作为参数（存入 a0、a1 寄存器中）传递给 `handle_interrupt()`。
2. 在 `handle_interrupt()` 中具体处理中断，对于无法处理的中断，根据 scause 打印错误信息并杀死当前线程并调度下一个线程。

RISC-V 中有两种中断发生时控制转移的方式，一种时直接跳转到 stvec 中指向的位置，一种是中断向量表。这里使用的是前者。

时钟中断发生时增加 `TICK`计数，每增加 100 个计数，打印信息。每次进入时钟中断处理处理函数都要设置下一次中断发生的时间。

`__interrupt()`函数中开启串口的几行代码看不懂。 



中断返回的过程:是中断发生过程的逆过程。

1. 将 sp 修改为调用 `handle_interrpt()` 时的堆栈指针，恢复 stval 和 scause 寄存器
2. 计算出中断发生时内核堆栈指针的值，存入 sscratch 寄存器中
3. 根据内核堆栈上存储的寄存器值，恢复通用寄存器和 sp



`rust_main()`函数中触发了断点后立刻终止程序，导致无法看到时钟中断的现象，所以我修改了`rust_main()`的代码，让 OS 进入无限循环观察时钟中断。

![](https://gitee.com/kongjun18/image/raw/master/Screenshot_20200723_172455.png)

## 阅读代码过程中提出的问题：

1. 开机运行 OS 时是什么模式？中断发生后是什么模式？中断返回时是什么模式？

> 开机后运行 openSBI，openSBI 将内核加载到内存 0x820000 上，并跳转到该处第一条指令执行（因此内核的第一条指令被设置为入口函数），进入内核态 （S态）。中断发生后进入内核态，中断返回时（`__restore()`中）仍然是内核态，处理器根据 status 寄存器中的 spp 域来判断返回到用户态还是留在内核态。



2中断默认实在 M 态处理的，如果需要在 S 态处理就要进行异常委托，异常委托在 lab 1 中有体现吗？

> 在 lab 1 的代码没有发现有看到有设置异常委托的代码。内核是由 openSBI 加载的，而且设置中断使能是调用的 SBI 的接口，所以猜测 openSBI 已经完成了异常委托。



## 实验题

1. 原理：在 `rust_main` 函数中，执行 `ebreak` 命令后至函数结束前，`sp` 寄存器的值是怎样变化的？

> 1. 控制转移到`__interrupt()`，`sp`被修改为`sscratch`寄存器指向的堆栈;
> 2. `sp`上移，空出来的空间用来存储`Context`，`sp`作为参数传递给`handle_interrupt()`函数;
> 3. `handle_interrupt()`函数中可能分配局部变量，`sp`可能再次上移，但是在函数返回时恢复原状;
> 4. 进入`__restore()`函数中，函数通过`sret`指令将堆栈中保存的`sp`（指向中断发生前的堆栈位置）恢复到`sp`寄存器中，并将控制转移到中断发生时要执行的指令。至此，`sp`恢复为中断发生前的状态。

2. 分析：如果去掉 `rust_main` 后的 `panic` 会发生什么，为什么？

> 会执行错误的代码，可能会出现程序崩溃，死循环和其他各种奇怪的问题。
>
> 删除`panic!`后，`rust_main`正常执行完毕后返回到入口函数中，入口函数之后还存在代码、数据，程序将错误地执行这些代码、数据，导致各种问题。

3. 实验
   1. 如果程序访问不存在的地址，会得到 `Exception::LoadFault`。模仿捕获 `ebreak` 和时钟中断的方法，捕获 `LoadFault`（之后 `panic` 即可）。
   2. 在处理异常的过程中，如果程序想要非法访问的地址是 `0x0`，则打印 `SUCCESS!`。

   > 实验问题 1、2 可以同时解答，代码如下：

   ```rust
   pub fn handle_interrupt(context: &mut Context, scause: Scause, stval: usize) {
       // 返回的 Context 必须位于放在内核栈顶
       match scause.cause() {
           // 断点中断（ebreak）
           Trap::Exception(Exception::Breakpoint) => breakpoint(context),
           // 新添加的代码
           Trap::Exception(Exception::LoadFault) => load_fault(context, stval),
           // 时钟中断
           Trap::Interrupt(Interrupt::SupervisorTimer) => supervisor_timer(context),
           // 其他情况，终止当前线程
           _ => fault(context, scause, stval),
       };
   }
   
   fn load_fault(context: &mut Context, stval: usize) {
       if stval == 0_usize {
           panic!("SUCCESS!");
       }
       panic!(
           "Error Type: LoadFault\nContext: {:?}\nstval: {:x}",
           context,
           stval,
       );
   }
   
   ```

   3. 添加或修改少量代码，使得运行时触发这个异常，并且打印出 `SUCCESS!`。要求：不允许添加或修改任何 unsafe 代码。

   > Safe Rust不会出现访问`0x00`这种无效地址的情况，因为不能使用 unsafe Rust，所以只能在不在 Rust 控制之下的地方做手脚。
   >
   > 从问题 2 可以看到，`rust_main()`运行结束后（删去`panic!`）会返回到入口函数执行，入口函数使用汇编语言写的，可以在入口函数中添加访问`0x00`的代码以触发异常。实际上直接删去`panic!`就可以触发异常了，`rust_main()`返回到`0x00`触发异常。
   >
   > 通过`sret`从中断中返回，返回地址就是`sepc`寄存器的值，可以修改`Context`中的保持的`sepc`或直接修改`sepc`寄存器使中断返回时跳转到`0x00`,触发异常。
   >
   > 我在`breakpoint()`函数中修改了`Context`中存储的`sepc`，分别使中断返回时跳转到`0x00`和`0x1111_1111`。运行结果如下：

   ![](https://gitee.com/kongjun18/image/raw/master/Screenshot_20200724_184535.png)

   ![](https://gitee.com/kongjun18/image/raw/master/Screenshot_20200724_184435.png)

   

# lab-2

## 实验题



1. 原理：.bss 字段是什么含义？为什么我们要将动态分配的内存（堆）空间放在 .bss 字段？

> .bss 是 *Block Started by Symbol* 的简称,属于古老的历史遗留问题。.bss 是 ELF 格式文件的一个段，存放初始化为 0 的全局变量和所有静态变量，因为这些变量的初始值已经确定，所以不占用可执行文件的空间。
>
> .bss 段不占用可执行文件的空间，而且在运行时分配空间且不会被回收，也不涉及物理内存管理器，所以是比较适合的堆空间。

2. 分析：我们在动态内存分配中实现了一个堆，它允许我们在内核代码中使用动态分配的内存，例如 `Vec` `Box` 等。那么，如果我们在实现这个堆的过程中使用 `Vec` 而不是 `[u8]`，会出现什么结果？

> 为了实现动态内存分配，必须先实现一个堆，为了实现堆又必须先实现动态内存分配，产生了循环依赖。

3. 基于线段树的内存分配器

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

运行结果:

![](https://gitee.com/kongjun18/image/raw/master/Screenshot_20200724_184952.png)

# lab-3

## 基础设施



`/os/src/memory/address.rs`中封装了物理页号、虚拟页号、物理地址、虚拟地址，并定义了一些操作。其中的操作主要分为三类：

1. 虚实转换
2. 解引用操作
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

## 临时页表：大页



## 物理内存分配器和建立映射的作用

lab-3 完成了从物理内存到虚拟内存的转换，所有的代码、数据都在虚拟内存的机制下访问。将一段虚拟内存映射至物理内存就意味着被映射的物理内存已经被使用，这段物理内存不能再被用于其他用途。为了保证这段物理内存不再被二次分配，操作系统必须知道哪些物理内存已经被分配。

rCore 内核被加载到了物理地址 `0x80200000`，内核占用的内存不在物理内存管理器的管理范围内，物理内存管理器对内核所在的内存区域毫不知情，只管理内核之上（`kernel_end`之后）的内存。物理内存管理的范围直接影响了完成虚拟内存到物理内存映射的关系。

对于操作系统内核，内核在开启虚拟内存之前就在物理内存中，而且地址是确定的，适合使用线性偏移的方式进行映射，将物理地址加上一个固定的偏移作为虚拟地址。在建立虚拟地址到物理地址的映射的过程中，内核所在的物理地址没有任何变化，也没有发生数据的复制，物理内存分配器的作用仅仅是分配物理页作为页表。

对于应用程序，使用的内存都在物理内存分配器的管理下，一半通过`Framed`方式进行虚拟地址映射，因为程序的指令、数据都不在物理内存中，需要分配多段物理内存并将可执行文件中的指令、数据分配到其中，所以建立映射的过程中发生了数据的复制，内存分配器不仅要分配内存作为页表，也要分配内存给应用程序。

## 实验题（待续）



# lab-4

内核栈：

时钟中断的变化：

线程切换：

## 提出的问题



2. `os/src/interrupt/mod.rs` 中 `wait_for_interrupt()`函数为什么先禁用时钟中断再开启全局中断，使用汇编指令 `wfi`，再关闭全局中断，开启时钟中断？

> 先禁用时钟中断再开启全局中断，时钟中断外的所有中断都可以正常发生，所以程序会等待，直到有除了时钟中断之外的中断发生。
>
> 关闭全局中断，开启时钟中断后，所有中断都不能发生，提前开启时钟中断应该只是为了避免下次打开全局中断时忘记打开时钟中断。



2. sscratch 的行为是怎样的？

> sscratch 始终指向内核堆栈，在中断发生时将 sscratch 的值赋给 sp;在中断处理完返回时将终端发生前内核堆栈指针恢复到 sscratch 中。



3. sscratch在哪里被初始化？

> `rust_main()`中创建了内核级进程/线程，并调用`run()`函数使其运行，`run()`将其`Context`结构压入到了内核栈最低部（最高地址），并`Context`结构的地址（指向内核堆栈的`Context`结构的指针）作为参数传递给`__restore()`，`__resotre()`将这个参数作为加上`36 * 8`字节得到内核堆栈起始位置，将其存入到 sscratch 寄存器中。这就是 sscratch 寄存器的初始化。

4. 可以在时钟中断发生之后在`run()`吗？

> 不可以。如果`run()`之前发生了时钟中断，中断时堆栈存储的`Context`会替代当前线程(`PROCESSOR.current_thread()`)的`Context`,线程存储的`Context`和线程对应的`Context`不一致。

## 实验题：（待续）