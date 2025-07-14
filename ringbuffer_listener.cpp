#include "ringbuffer_listener.h"
#include <iostream>
#include <unistd.h>
#include <iomanip>

#include "feature_engine.h"

static EventCallback callback_fn;

static int handle_event(void *ctx, void *data, size_t len) {
    if (len < sizeof(task_event)) {
        std::cerr << "[Error] Received event too small: " << len << " bytes\n";
        return 0;
    }

    auto *evt = static_cast<task_event*>(data);

    //  Debug 印出完整欄位
    std::cout << "[RingBuffer] Event received:" << std::endl;
    std::cout << "  PID:            " << evt->pid << std::endl;
    std::cout << "  Priority:       " << evt->prio << std::endl;
    std::cout << "  CPUs Allowed:   " << evt->nr_cpus_allowed << std::endl;
    std::cout << "  Runtime (ns):   " << evt->runtime_ns << std::endl;
    std::cout << "  VRuntime:       " << evt->vruntime << std::endl;
    std::cout << "  Ctx Switch:     " << evt->switch_count << std::endl;
    std::cout << "  Timestamp (ns): " << evt->timestamp_ns << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    // 呼叫 callback（例如給 FeatureEngine 推論）
    if (callback_fn)
        callback_fn(*evt);

    return 0;
}

void run_ringbuffer_listener(struct bpf_map *ringbuf, EventCallback cb) {
    callback_fn = cb;
    struct ring_buffer *rb = ring_buffer__new(bpf_map__fd(ringbuf), handle_event, nullptr, nullptr);

    if (!rb) {
        std::cerr << "[RingBuffer] Failed to create ring buffer\n";
        return;
    }

    std::cout << "[RingBuffer] Listener started...\n";

    while (true) {
        int ret = ring_buffer__poll(rb, 100 /* ms */);
        if (ret < 0) {
            std::cerr << "[RingBuffer] Polling error: " << ret << std::endl;
        }
    }
}

