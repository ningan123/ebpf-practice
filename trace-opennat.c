#include <uapi/linux/ptrace.h>
#include <uapi/linux/limits.h>
#include <linux/sched.h>
// define the output data structure.
struct data_t {
	u32 pid;
	u64 ts;
	char comm[TASK_COMM_LEN];
	char fname[NAME_MAX];
};
BPF_PERF_OUTPUT(events);
TRACEPOINT_PROBE(syscalls, sys_enter_openat)
{
	struct data_t data = { };
	data.pid = bpf_get_current_pid_tgid();
	data.ts = bpf_ktime_get_ns();
	if (bpf_get_current_comm(&data.comm, sizeof(data.comm)) == 0) {
		bpf_probe_read_user(&data.fname, sizeof(data.fname), args->filename);
	}
	events.perf_submit(args, &data, sizeof(data));
	return 0;
}