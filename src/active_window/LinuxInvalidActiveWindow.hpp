#ifndef GUARD_LinuxInvalidActiveWindow_h
#define GUARD_LinuxInvalidActiveWindow_h

class LinuxInvalidActiveWindow : public IActiveWindow
{
    public:
        LinuxInvalidActiveWindow()
        {
            name_ = "";
            process_name_ = "";
        }

        virtual std::string get_name() { return name_; }
        virtual std::string get_process_name() { return process_name_; }
        virtual int get_cpu_usage() { return 0; }
};

#endif
