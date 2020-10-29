```shell
2020-09-26T12:39:46+08:00
Running ./write-benchmark
Run on (8 X 4200 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x4)
  L1 Instruction 32 KiB (x4)
  L2 Unified 256 KiB (x4)
  L3 Unified 6144 KiB (x1)
Load Average: 0.54, 0.86, 0.95
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
----------------------------------------------------------
Benchmark                Time             CPU   Iterations
----------------------------------------------------------
BM_SyncWrite    441126596925 ns   57386798130 ns            1
BM_NonsyncWrite 320670263158 ns   26945443162 ns            1

```

*UNIX system advanced programming* said "The Linux operating system isn't allowing us to set the O_SYNC flag using fcntl instead failing without returning an error". In manual pages of Linux 5.8, This

