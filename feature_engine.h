#pragma once
#include <vector>
#include "task_event.h"

class FeatureEngine {
public:
    void pushEvent(const task_event &event);
    bool ready() const;
    std::vector<std::vector<float>> get_sequence() const;
    void clear();
private:
    std::vector<task_event> buffer;
    static const int WINDOW_SIZE = 8;
};

