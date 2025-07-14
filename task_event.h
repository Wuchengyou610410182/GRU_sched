#ifndef __TASK_EVENT_H__
#define __TASK_EVENT_H__

#include <linux/types.h>

struct task_event {
    __u32 pid;               // Process ID
    __u32 prio;              // Static priority
    __u32 nr_cpus_allowed;   // Allowed CPU mask count
    __u64 runtime_ns;        // Runtime in nanoseconds
    __u64 nvcsw;             // Voluntary context switches
    __u64 total_exec_runtime; // Total execution time
    __u64 switch_count;      // Custom context switch count
    __u64 vruntime;          // Virtual runtime
    __u64 timestamp_ns;      // Time of event emission
};

#endif /* __TASK_EVENT_H__ */

