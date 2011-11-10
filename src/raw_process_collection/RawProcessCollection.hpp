#ifndef GUARD_RawProcessCollection_h
#define GUARD_RawProcessCollection_h

#include <map>
#include <memory>
#include <string>
#include <vector>

class ProcessInformation
{
    public:
        std::string name;
        unsigned long long before_time;
        unsigned long long after_time;
        unsigned long long ram_usage;
        unsigned int ppid;
    private:
        friend class RawProcessCollection;
        unsigned int last_update_run_;
};

class RawProcessCollection
{
    public:

        typedef std::map<unsigned int, ProcessInformation>::const_iterator
            const_iterator;

        const_iterator begin() const { return processes_.begin(); }
        const_iterator end() const { return processes_.end(); }

        // NOTE: must call init followed by update after a one second delay
        void init()
        {
            std::unique_ptr<std::vector<unsigned int>> pids = get_pids_();
            for(std::vector<unsigned int>::const_iterator pid = pids->begin();
                pid != pids->end(); ++pid) {

                processes_[*pid].before_time = get_process_time_(*pid);
            }
            current_update_run_ = 1;
        }

        void update();

        bool contains_pid(unsigned int pid) const
        {
            return processes_.find(pid) != processes_.end();
        }

        unsigned int get_ppid(unsigned int pid) const
        {
            return processes_.at(pid).ppid;
        }

    protected:

        std::map<unsigned int, ProcessInformation> processes_;

        // Used to determine which update RawProcessCollection is on.
        // Signals expired processes to be removed from processes.
        // Incremented on each run of update
        unsigned int current_update_run_;

        unsigned int set_last_update_run_(
            ProcessInformation* info,
            unsigned int last_update_run
        ) {
            return info->last_update_run_ = last_update_run;
        }

        virtual std::unique_ptr<std::vector<unsigned int>>
        get_pids_() const = 0;

        virtual unsigned long long get_process_time_(unsigned int pid)
        const = 0;

        virtual unsigned long long get_process_ram_usage_(unsigned int pid)
        const = 0;

        // Populates entries for name, after_time, ram_usage, and ppid
        virtual void platform_specific_update_() = 0;
};

#endif
