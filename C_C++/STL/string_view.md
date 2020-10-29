优势：

- 不需要内存分配，可以大幅度提升性能，尤其是`substr()`
- 避免临时对象的生成

陷阱：

- string_view 没有所有权
- 不是 null-terminated 的

