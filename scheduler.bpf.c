// SPDX-License-Identifier: Dual BSD/GPL
#define FALLBACK_DSQ 4242

#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_core_read.h>
#include <bpf/bpf_tracing.h>
#include <scx/common.bpf.h>
#include <scx/compat.bpf.h>

#include "task_event.h"
#include "maps.h"

char LICENSE[] SEC("license") = "Dual BSD/GPL";

// === 全域參數 ===
const volatile s32 CENTRAL_CPU = 0;

// === sched_switch 監聽：統計 context switch 次數 ===
s32 BPF_KPROBE(trace_sched_switch, struct task_struct *prev, struct task_struct *next) {
    u32 pid = BPF_CORE_READ(next, pid);
    u64 one = 1;
    bpf_map_update_elem(&switch_count, &pid, &one, BPF_ANY);
    return 0;
}

// === CPU 選擇邏輯（使用預設）===
s32 BPF_STRUCT_OPS(gru_select_cpu, struct task_struct *p, s32 prev_cpu, u64 wake_flags) {
    bool is_idle = false;
    return scx_bpf_select_cpu_dfl(p, prev_cpu, wake_flags, &is_idle);
}


// === enqueue：擷取任務特徵並送至 ringbuffer ===
void BPF_STRUCT_OPS(gru_enqueue, struct task_struct *p, u64 enq_flags) {
    struct task_event evt = {};
    u32 pid = p->pid;

    evt.pid = pid;
    evt.prio = p->prio;
    evt.nr_cpus_allowed = p->nr_cpus_allowed;
    evt.runtime_ns = p->se.sum_exec_runtime;
    evt.vruntime = p->se.vruntime;

    u64 *sw = bpf_map_lookup_elem(&switch_count, &pid);
    evt.switch_count = sw ? *sw : 0;
    evt.timestamp_ns = bpf_ktime_get_ns();

    // ✅ Kernel log：檢查是否觸發
    bpf_printk("enqueue: pid=%d, prio=%d, exec=%.2llu", evt.pid, evt.prio, evt.runtime_ns);

    // 寫入 ringbuffer
    bpf_ringbuf_output(&event_ringbuf, &evt, sizeof(evt), 0);

    // 放進 dispatch queue
    scx_bpf_dsq_insert(p, FALLBACK_DSQ, SCX_SLICE_DFL, enq_flags);
}

// === dispatch：當 CPU idle，從 fallback queue 中選任務 ===
void BPF_STRUCT_OPS(gru_dispatch, s32 cpu, struct task_struct *prev) {
    if (cpu == CENTRAL_CPU)
        scx_bpf_dsq_move_to_local(FALLBACK_DSQ);
}

// ✅ init：建立 fallback DSQ（無 switch_all）
s32 BPF_STRUCT_OPS_SLEEPABLE(gru_init) {
    return scx_bpf_create_dsq(FALLBACK_DSQ, 0);
}

// === 結束清理 ===
void BPF_STRUCT_OPS(gru_exit, struct scx_exit_info *ei) {
    return;
}

// === sched_ext 描述器 ===
SEC(".struct_ops")
struct sched_ext_ops gru_sched_ops = {
    .select_cpu = (void *)gru_select_cpu,
    .enqueue    = (void *)gru_enqueue,
    .dispatch   = (void *)gru_dispatch,
    .init       = (void *)gru_init,
    .exit       = (void *)gru_exit,
    .name       = "gru_sched",
};

