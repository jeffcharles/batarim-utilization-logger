#ifndef GUARD_LinuxActiveWindow_h
#define GUARD_LinuxActiveWindow_h

#include <ios>
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
            name_ = "";
            process_name_ = "";
        }

        virtual std::string& get_name();
        virtual std::string& get_process_name();
    
    private:
        std::shared_ptr<Display> display_;
        Window window_;
};

#endif
