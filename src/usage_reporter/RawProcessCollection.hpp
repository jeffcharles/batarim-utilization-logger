#ifndef GUARD_RawProcessCollection_h
#define GUARD_RawProcessCollection_h

#include <map>
#include <memory>
#include <vector>

struct ProcessInformation
{
    unsigned long long before_time;
    unsigned long long after_time;
    unsigned long long ram_usage;
};

class RawProcessCollection
{
    public:

        // NOTE: must call init followed by update after a one second delay
        void init()
        {
            std::shared_ptr<std::vector<unsigned int>> pids = get_pids_();
            for(std::vector<unsigned int>::const_iterator pid = pids->begin();
                pid != pids->end(); ++pid) {

                processes_[*pid].before_time = get_process_time_(*pid);
            }
        }

        void update();

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

    protected:

        virtual std::shared_ptr<std::vector<unsigned int>>
        get_pids_() const = 0;

        virtual unsigned long long get_process_time_(unsigned int pid)
        const = 0;

        virtual unsigned long long get_process_ram_usage_(unsigned int pid)
        const = 0;

    private:
        
        std::map<unsigned int, ProcessInformation> processes_;
        
};

#endif
