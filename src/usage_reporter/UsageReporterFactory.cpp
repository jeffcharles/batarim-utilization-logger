#include <memory>

#include "UsageReporter.hpp"

#ifdef WIN32
#include "WindowsUsageReporter.hpp"
#endif

#ifdef LINUX
#include "LinuxUsageReporter.hpp"
#endif

using std::shared_ptr;

shared_ptr<UsageReporter> get_usage_reporter()
{
#ifdef WIN32
    return shared_ptr<UsageReporter>(new WindowsUsageReporter());
#endif

#ifdef LINUX
    return shared_ptr<UsageReporter>(new LinuxUsageReporter());
#endif
}
