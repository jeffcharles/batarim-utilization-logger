#ifndef GUARD_ProcessList_h
#define GUARD_ProcessList_h

#include <algorithm>
#include <map>
#include <memory>
#include <vector>

struct ProcessInformation
{
    unsigned long long before_time;
    unsigned long long after_time;
    unsigned long long ram_usage;
};

namespace {
    unsigned int get_pid_from_pair_(
        const std::pair<unsigned int, ProcessInformation>& proc_pair
    ) {
        return proc_pair.first;
    }
}

class ProcessList
{
    public:

        unsigned int get_pid_with_highest_cpu_usage() const;

        unsigned int get_pid_with_highest_ram_usage() const;

        unsigned long long get_time(unsigned int pid) const
        {
            ProcessInformation process = processes_.at(pid);
            return process.after_time - process.before_time;
        }
        
        unsigned long long get_ram(unsigned int pid) const
        {
            return processes_.at(pid).ram_usage;
        }

        std::shared_ptr<std::vector<unsigned int>> get_pids() const
        {
            std::shared_ptr<std::vector<unsigned int>> pids(
                new std::vector<unsigned int>());
            pids->resize(processes_.size());
            
            std::transform(processes_.begin(), processes_.end(), pids->begin(),
                get_pid_from_pair_);
            
            return pids;
        }

        void set_before_time(unsigned int pid, unsigned long long time)
        {
            processes_[pid].before_time = time;
        }

        void set_after_time(unsigned int pid, unsigned long long time)
        {
            processes_[pid].after_time = time;
        }

        void set_ram_usage(unsigned int pid, unsigned long long ram)
        {
            processes_[pid].ram_usage = ram;
        }

    private:
        std::map<unsigned int, ProcessInformation> processes_;
};

#endif
