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

        typedef std::map<unsigned int, ProcessInformation>::const_iterator
            const_iterator;

        const_iterator begin() const { return processes_.begin(); }
        const_iterator end() const { return processes_.end(); }

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
