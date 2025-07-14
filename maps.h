// include/maps.h
#ifndef __MAPS_H__
#define __MAPS_H__

#include "task_event.h"

struct {
    __uint(type, BPF_MAP_TYPE_RINGBUF);
    __uint(max_entries, 1 << 24);
} event_ringbuf SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 10240);
    __type(key, u32);
    __type(value, u64);
} switch_count SEC(".maps");

#endif


