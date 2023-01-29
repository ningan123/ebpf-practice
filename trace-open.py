from bcc import BPF

# 1) load BPF program
# 加载 eBPF 程序并挂载到内核探针上
b = BPF(src_file="trace-open.c")
b.attach_kprobe(event="do_sys_openat2", fn_name="hello_world")

# 2) print header
# 输出一行 Header 字符串表示数据的格式
print("%-18s %-16s %-6s %-16s" % ("TIME(s)", "COMM", "PID", "FILE"))

# 3) define the callback for perf event
# print_event 定义一个数据处理的回调函数，打印进程的名字、PID 以及它调用 openat 时打开的文件
start = 0
def print_event(cpu, data, size):
    global start
    event = b["events"].event(data)
    if start == 0:
            start = event.ts
    time_s = (float(event.ts - start)) / 1000000000
    print("%-18.9f %-16s %-6d %-16s" % (time_s, event.comm, event.pid, event.fname))

# 4) loop with callback to print_event
# open_perf_buffer 定义了名为 “events” 的 Perf 事件映射，而后通过一个循环调用 perf_buffer_poll 读取映射的内容，并执行回调函数输出进程信息
b["events"].open_perf_buffer(print_event)
while 1:
    try:
        b.perf_buffer_poll()
    except KeyboardInterrupt:
        exit()


# 怎样从用户态读取 BPF 映射内容并输出到标准输出（stdout）呢？
# 在 BCC 中，与 eBPF 程序中 BPF_PERF_OUTPUT  相对应的用户态辅助函数是 open_perf_buffer() 。它需要传入一个回调函数，用于处理从 Perf 事件类型的 BPF 映射中读取到的数据。