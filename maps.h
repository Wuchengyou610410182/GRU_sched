// include/maps.h

#ifndef MAPS_H
#define MAPS_H

#include <bpf/bpf_helpers.h>
#include "task_event.h"   // ✅ 確保 struct task_event 有被包含

// ✅ event_ringbuf：使用明確 flags，避免 Invalid Argument 錯誤
struct {
    __uint(type, BPF_MAP_TYPE_RINGBUF);
    __uint(max_entries, 1 << 20);       // 1MB buffer，夠用了
    __uint(map_flags, 0);               // 若支援 mmap 可設為 BPF_F_MMAPABLE
} event_ringbuf SEC(".maps");

// ✅ switch_count：儲存 PID -> switch count
struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);     // PID
    __type(value, u64);   // switch count
} switch_count SEC(".maps");

#endif // MAPS_H

