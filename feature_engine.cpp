#include "feature_engine.h"

void FeatureEngine::pushEvent(const task_event &evt) {
    if (buffer.size() >= WINDOW_SIZE)
        buffer.erase(buffer.begin());
    buffer.push_back(evt);
}

bool FeatureEngine::ready() const {
    return buffer.size() == WINDOW_SIZE;
}

std::vector<std::vector<float>> FeatureEngine::get_sequence() const {
    std::vector<std::vector<float>> seq;
    for (const auto& evt : buffer) {
        seq.push_back({
            (float)evt.prio,
            (float)evt.nr_cpus_allowed,
            (float)evt.runtime_ns,
            (float)evt.vruntime,
            (float)evt.switch_count,
            (float)evt.timestamp_ns,
            (float)evt.pid
        });
    }
    return seq;
}

void FeatureEngine::clear() {
    buffer.clear();
}

