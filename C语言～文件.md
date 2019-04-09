# C语言～文件
1. **文件相关原理**
2. **文本I/O**
3. **二进制I/O**
4. **其他文件函数**
## 文件相关原理
### 文本文件与二进制文件


## 文本I/O

## 二进制I/O ：fread()和fwrite()
二进制I/O面向二进制数据，用于处理数值数据。二进制I/O使用与计算机相同的位组合来存储数值，保证了数值在存储前后的一致。
### fwrite()
 **size_t** fwrite(const void * restrict **ptr**, size_t **size**, size_t **nmemb**, FILE * restrict **fp**)

1. ptr为待写入数据对象的地址，size为数据块的大小（以**字节**为单位），nmemb表示写入数据块的个数，fp是目标文件的指针。
2. fwrite()函数返回成功写入ptr是数据块个数。正常情况下，返回值应为nmemb；异常情况下，返回值小于nmemb。
```
//将一个内含十个double型数据的数组写入文件
double earnings[10];
fwrite(earnings,sizeof(double),10,fp);
```
### fread
**size_t** fread(const void * restrict **ptr**, size_t **size**, size_t **nmemb**, FILE * restrict **fp**)
66. ptr为数据对象在内存中的地址，size为数据块大小，nmemb为数据块个数，fp
为来源文件。
2. fread()函数用于读取fwrite()函数写入到文件中的数据，读取来源文件的数据到ptr中。
3. fread()函数会返回正常读取项的数量。正常情况下，返回值等于nmemb；异常情况下，返回值小于nmemb。

```
//恢复上面earnings数组
double earnings[10];
fread(earnings,sizeof(double),10,fp);
```





