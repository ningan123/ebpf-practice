
// 追踪 bpf 系统调用，借助 BCC 宏定义 TRACEPOINT_PROBE(category, event) 比较方便

TRACEPOINT_PROBE(syscalls, sys_enter_bpf)
{
    bpf_trace_printk("%d\\n", args->cmd);
    return 0;
}