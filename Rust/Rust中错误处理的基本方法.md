# Rust中错误处理的基本方法

##  unwrap()系列

`Option<T>`和`Result<T,E>`都内置了`unwrap()`系列函数用于提取`T`成员并进行错误情况的处理。

`unwrap()` ：若 `Option<T>`为`Some<T>`或`Result<T,E>`为`Ok(T)`，则提取出`T`;若 `Option<T>`为`None或`Result<T,E>为`Err（E）`则 panic

`unwrap_or(default)`:若 `Option<T>`为`Some<T>`或`Result<T,E>`为`Ok(T)`，则提取出`T`;否则给一个默认值`default`

`unwrap_or_else(fnOnce()):`若 `Option<T>`为`Some<T>`或`Result<T,E>`为`Ok(T)`，则提取出`T`;否则调用闭包`fnOnce()`

对于`Option<T>`还有`unwrap_none()`。当`Option<T>`为`Some<T>`时返回`T`，是`None`时消耗掉`Option<T>`，返回任何值。

对于`Result<T,E>`还有`unwrap_err()`，该方法与`unwrap()`恰好功能相反。当`Result<T,E>`是`Ok(T)`时直接 panic，是`Err(E)`时返回`E`。



## map()系列

`map()`系列方法用于将一个类型的`Option`转换为另一个类型的`Option`，将一个类型的`Result`转换成另一个类型的`Result`，并修改它们的值。这一系列函数称作组合子。

`map()`的语义将一个`Option`映射为另一个`Option`或将一个`Result`映射为另一个`Result`，

Programming in Lua by Roberto Ierusalimschy (z-lib.org)