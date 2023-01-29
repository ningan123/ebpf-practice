int hello_world(void *ctx)
{
    bpf_trace_printk("Hello, World!");  // 最常用的 BPF 辅助函数，它的作用是输出一段字符串。不过，由于 eBPF 运行在内核中，它的输出并不是通常的标准输出（stdout），而是内核调试文件 /sys/kernel/debug/tracing/trace_pipe 
    return 0;
}

// eBPF程序并不能随便的调用内核函数，必须通过辅助函数才可完成eBPF程序和其他内核模块的交互，eg bpf_trace_printk()
