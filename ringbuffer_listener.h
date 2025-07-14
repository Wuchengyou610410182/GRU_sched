#pragma once
#include <functional>
#include <bpf/libbpf.h>
#include "task_event.h"

using EventCallback = std::function<void(const task_event&)>;

void run_ringbuffer_listener(struct bpf_map *ringbuf, EventCallback cb);

