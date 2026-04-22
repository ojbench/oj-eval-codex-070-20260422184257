#ifndef SRC_HPP
#define SRC_HPP

#include <vector>
#include <utility>

using namespace std;

typedef unsigned int uint;

namespace sjtu {
    struct Task {
        uint task_id;
        uint priority;
        uint time;

        explicit Task(uint _task_id = 0, uint _priority = 0, uint _time = 0)
            : task_id(_task_id), priority(_priority), time(_time) {}

        Task(const Task &rhs) : task_id(rhs.task_id), priority(rhs.priority), time(rhs.time) {}

        ~Task() = default;
    };

    enum CPUState { idle = 0, busy = 1 };

    class CPU {
    protected:
        CPUState state;
        vector<Task> tasks;

    public:
        CPU() : state(idle), tasks() {}

        int addTask(const Task &t) {
            tasks.push_back(t);
            return 1;
        }

        int changePriority(uint task_id, uint priority) {
            for (auto &task : tasks)
                if (task.task_id == task_id) {
                    task.priority = priority;
                    return 1;
                }
            return 0;
        }

        virtual pair<CPUState, uint> run() = 0;

        virtual ~CPU() = default;
    };

    // FCFS: execute first-added task; if it finishes, remove it.
    class CPU_FCFS : public CPU {
    public:
        pair<CPUState, uint> run() override {
            // Clean finished leading tasks (defensive)
            while (!tasks.empty() && tasks.front().time == 0) {
                tasks.erase(tasks.begin());
            }
            if (tasks.empty()) {
                state = idle;
                return make_pair(state, 0u);
            }
            state = busy;
            uint id = tasks.front().task_id;
            // Execute 1 time unit
            tasks.front().time -= 1;
            if (tasks.front().time == 0) {
                tasks.erase(tasks.begin());
            }
            return make_pair(state, id);
        }
    };

    // SRTF: pick task with smallest remaining time; tie -> earliest arrival.
    class CPU_SRTF : public CPU {
    public:
        pair<CPUState, uint> run() override {
            int best = -1;
            for (int i = 0; i < (int)tasks.size(); ++i) {
                if (tasks[i].time == 0) continue;
                if (best == -1 || tasks[i].time < tasks[best].time) {
                    best = i;
                }
            }
            if (best == -1) {
                state = idle;
                return make_pair(state, 0u);
            }
            state = busy;
            uint id = tasks[best].task_id;
            tasks[best].time -= 1;
            if (tasks[best].time == 0) {
                tasks.erase(tasks.begin() + best);
            }
            return make_pair(state, id);
        }
    };

    // Priority: pick smallest priority value; tie -> earliest arrival.
    class CPU_PRIORITY : public CPU {
    public:
        pair<CPUState, uint> run() override {
            int best = -1;
            for (int i = 0; i < (int)tasks.size(); ++i) {
                if (tasks[i].time == 0) continue;
                if (best == -1 || tasks[i].priority < tasks[best].priority) {
                    best = i;
                }
            }
            if (best == -1) {
                state = idle;
                return make_pair(state, 0u);
            }
            state = busy;
            uint id = tasks[best].task_id;
            tasks[best].time -= 1;
            if (tasks[best].time == 0) {
                tasks.erase(tasks.begin() + best);
            }
            return make_pair(state, id);
        }
    };
}

#endif
