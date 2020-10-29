# 利用模板进行编译期运算

# constexpr function

# constexpr variable

# compiler support

```shell
[I] kongjun@Friday ~/D/p/cpp_practice [SIGINT]> time g++ -fconstexpr-ops-limit=1000000000000 -std=c++20 -o constexpr constexpr.cpp
g++: 致命错误：已杀死 signal terminated program cc1plus
编译中断。

________________________________________________________
Executed in  432.76 secs   fish           external 
   usr time  415.85 secs    0.07 millis  415.85 secs 
   sys time   13.04 secs    2.93 millis   13.04 secs 


```

# GNU extension: pure function and const function

