#ifndef SRC_HPP
#define SRC_HPP

namespace sjtu {
    class CPU_FCFS : public CPU {
    public:
        std::pair<CPUState, unsigned int> run() override {
            while (tasks.size() > 0 && tasks.front().time == 0) {
                tasks.erase(tasks.begin());
            }
            if (tasks.size() == 0) {
                state = idle;
                return std::make_pair(state, 0u);
            }
            state = busy;
            unsigned int id = tasks.front().task_id;
            tasks.front().time = tasks.front().time - 1;
            if (tasks.front().time == 0) {
                tasks.erase(tasks.begin());
            }
            return std::make_pair(state, id);
        }
    };

    class CPU_SRTF : public CPU {
    public:
        std::pair<CPUState, unsigned int> run() override {
            int best = -1;
            for (int i = 0; i < (int)tasks.size(); ++i) {
                if (tasks[i].time == 0) continue;
                if (best == -1 || tasks[i].time < tasks[best].time) {
                    best = i;
                }
            }
            if (best == -1) {
                state = idle;
                return std::make_pair(state, 0u);
            }
            state = busy;
            unsigned int id = tasks[best].task_id;
            tasks[best].time = tasks[best].time - 1;
            if (tasks[best].time == 0) {
                tasks.erase(tasks.begin() + best);
            }
            return std::make_pair(state, id);
        }
    };

    class CPU_PRIORITY : public CPU {
    public:
        std::pair<CPUState, unsigned int> run() override {
            int best = -1;
            for (int i = 0; i < (int)tasks.size(); ++i) {
                if (tasks[i].time == 0) continue;
                if (best == -1 || tasks[i].priority < tasks[best].priority) {
                    best = i;
                }
            }
            if (best == -1) {
                state = idle;
                return std::make_pair(state, 0u);
            }
            state = busy;
            unsigned int id = tasks[best].task_id;
            tasks[best].time = tasks[best].time - 1;
            if (tasks[best].time == 0) {
                tasks.erase(tasks.begin() + best);
            }
            return std::make_pair(state, id);
        }
    };
}

#endif
