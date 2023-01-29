#!/usr/bin/env python3
# 1) import bcc library
# 导入了 BCC  库的 BPF 模块，以便接下来调用；
from bcc import BPF

# 2) load BPF program
# 调用 BPF() 加载第一步开发的 BPF 源代码；
b = BPF(src_file="hello.c")

# 3) attach kprobe
# 将 BPF 程序挂载到内核探针（简称 kprobe），其中 do_sys_openat2() 是系统调用 openat() 在内核中的实现；
# 调用了 attach_kprobe 函数，绑定了一个内核跟踪事件
b.attach_kprobe(event="do_sys_openat2", fn_name="hello_world")


# 4) read and print /sys/kernel/debug/tracing/trace_pipe
# 读取内核调试文件 /sys/kernel/debug/tracing/trace_pipe 的内容，并打印到标准输出中。
b.trace_print()


# 内核函数 do_sys_openat2

# 理解：opennat()和opennat2()都是系统调用
# 在计算机中运行程序、读写文件，都会涉及到文件的打开操作，Linux v5.10 与文件打开相关的系统调用有 open() / creat() / openat() / openat2这四类，在使用 glibc v2.32 时，几乎所有的文件打开操作使用的都是 openat2() 这个系统调用。

# 用高级语言开发的 eBPF 程序，需要首先编译为 BPF 字节码，然后借助 bpf 系统调用加载到内核中，最后再通过性能监控等接口与具体的内核事件进行绑定。这样，内核的性能监控模块才会在内核事件发生时，自动执行我们开发的 eBPF 程序。