// 包含头文件
#include <uapi/linux/openat2.h>
#include <linux/sched.h>

// 定义数据结构
struct data_t {
  u32 pid;
  u64 ts;
  char comm[TASK_COMM_LEN];
  char fname[NAME_MAX];
};

// 定义性能事件映射  定义一个 Perf 事件类型的 BPF 映射
BPF_PERF_OUTPUT(events);



// 定义kprobe处理函数
int hello_world(struct pt_regs *ctx, int dfd, const char __user * filename, struct open_how *how)
{
  struct data_t data = { };

  // 获取PID和时间
  data.pid = bpf_get_current_pid_tgid();  // bpf_get_current_pid_tgid用于获取进程的 TGID 和 PID。因为这儿定义的 data.pid 数据类型为 u32，所以高 32 位舍弃掉后就是进程的 PID
  data.ts = bpf_ktime_get_ns();  // bpf_ktime_get_ns用于获取系统自启动以来的时间，单位是纳秒

  // 获取进程名
  if (bpf_get_current_comm(&data.comm, sizeof(data.comm)) == 0)  // bpf_get_current_comm用于获取进程名，并把进程名复制到预定义的缓冲区中
  {
    bpf_probe_read(&data.fname, sizeof(data.fname), (void *)filename); // bpf_probe_read 用于从指定指针处读取固定大小的数据，这里则用于读取进程打开的文件名。
  }

  // 提交性能事件 调用 perf_submit() 把数据提交到刚才定义的 BPF 映射
  events.perf_submit(ctx, &data, sizeof(data));
  return 0;
}
