#include <memory>

#include "UsageReporter.hpp"

#ifdef WIN32
#include "WindowsUsageReporter.hpp"
#endif

#ifdef LINUX
#include "LinuxUsageReporter.hpp"
#endif

using std::unique_ptr;

unique_ptr<UsageReporter> get_usage_reporter()
{
#ifdef WIN32
    return unique_ptr<UsageReporter>(new WindowsUsageReporter());
#endif

#ifdef LINUX
    return unique_ptr<UsageReporter>(new LinuxUsageReporter());
#endif
}
