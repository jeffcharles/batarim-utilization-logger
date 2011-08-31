#ifndef GUARD_WindowsActiveWindow_h
#define GUARD_WindowsActiveWindow_h

#include <string>

#include "../usage_reporter/IUsageReporterRequestCollectionSetter.hpp"

#include "IActiveWindow.hpp"

class WindowsActiveWindow : public IActiveWindow
{
    public:
        WindowsActiveWindow();

        virtual std::string get_name() { return name_; }
        virtual std::string get_process_name();
        virtual void populate_cpu_usage(
            IUsageReporterRequestCollectionSetter& request_collection,
            int* cpu_usage
        );

    private:
        std::string get_filename_from_win32_path(std::string& path);
};

#endif