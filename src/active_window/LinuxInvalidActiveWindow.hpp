#ifndef GUARD_LinuxInvalidActiveWindow_h
#define GUARD_LinuxInvalidActiveWindow_h

class LinuxInvalidActiveWindow : public IActiveWindow
{
    public:
        LinuxInvalidActiveWindow()
        {
            name_ = L"";
            process_name_ = L"";
        }

        virtual std::wstring get_name() { return name_; }
        virtual std::wstring get_process_name() { return process_name_; }
        virtual int get_cpu_usage() { return 0; }
};

#endif
