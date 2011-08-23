#ifndef GUARD_LinuxActiveWindow_h
#define GUARD_LinuxActiveWindow_h

#include <memory>
#include <string>

#include <X11/Xlib.h>

#include "IActiveWindow.hpp"

class LinuxActiveWindow : public IActiveWindow
{
    public:
        LinuxActiveWindow(
            std::shared_ptr<Display> display,
            Window window,
            unsigned int pid
        ):
            display_(display),
            window_(window)
        {
            pid_ = pid;
            name_ = L"";
            process_name_ = L"";
            cpu_usage_ = 0;
        }

        virtual std::wstring get_name();
        virtual std::wstring get_process_name();
        virtual int get_cpu_usage();

    private:
        std::shared_ptr<Display> display_;
        Window window_;
};

#endif
