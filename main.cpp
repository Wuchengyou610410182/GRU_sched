#include <iostream>
#include <csignal>
#include <unistd.h>
#include <thread>

#include <bpf/libbpf.h>
#include <bpf/bpf.h>

// ✅ 額外定義，避免 sched.skel.h 編譯錯誤
typedef int32_t s32;
typedef uint64_t u64;

// ✅ BPF skeleton header
#include "sched.skel.h"

// ✅ 事件格式 + RingBuffer listener
#include "task_event.h"
#include "ringbuffer_listener.h"

static volatile bool exiting = false;

void handle_signal(int sig) {
    exiting = true;
}

int main() {
    struct scheduler_bpf *skel;

    // ✅ 開啟 BPF skeleton
    skel = scheduler_bpf__open();
    if (!skel) {
        std::cerr << "❌ Failed to open BPF skeleton" << std::endl;
        return 1;
    }

    // ✅ 載入 BPF 程式
    if (scheduler_bpf__load(skel)) {
        std::cerr << "❌ Failed to load BPF skeleton" << std::endl;
        scheduler_bpf__destroy(skel);
        return 1;
    }

    // ✅ Attach 程式
    if (scheduler_bpf__attach(skel)) {
        std::cerr << "❌ Failed to attach BPF skeleton" << std::endl;
        scheduler_bpf__destroy(skel);
        return 1;
    }

    std::cout << "✅ BPF program loaded and attached." << std::endl;

    // ✅ 處理 Ctrl+C
    signal(SIGINT, handle_signal);

    // ✅ 啟動 ringbuffer listener，列印事件
    std::thread listener_thread(run_ringbuffer_listener,
                                skel->maps.event_ringbuf,
                                [](const task_event &evt) {
                                    std::cout << "📥 Event received: "
                                              << "PID=" << evt.pid
                                              << " PRIO=" << evt.prio
                                              << " TS=" << evt.timestamp_ns
                                              << std::endl;
                                });

    while (!exiting) {
        sleep(1);
    }

    listener_thread.detach(); // 清理：此處可改用 join() 視程式用途而定
    scheduler_bpf__destroy(skel);
    std::cout << "👋 Exiting." << std::endl;
    return 0;
}

