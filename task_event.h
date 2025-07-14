#ifndef __TASK_EVENT_H__
#define __TASK_EVENT_H__



struct task_event {
    __u32 pid;
    __u32 prio;
    __u32 nr_cpus_allowed;
    __u64 runtime_ns;
    __u64 nvcsw;
    __u64 total_exec_runtime;
    __u64 switch_count;
    __u64 vruntime;
    __u64 timestamp_ns;
};

#endif /* __TASK_EVENT_H__ */


