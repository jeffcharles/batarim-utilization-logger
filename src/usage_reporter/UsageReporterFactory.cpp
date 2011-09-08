#include <memory>

#include "UsageReporter.hpp"

#ifdef WIN32
#include "WindowsUsageReporter.hpp"
#endif

using std::shared_ptr;

shared_ptr<UsageReporter> get_usage_reporter()
{
#ifdef WIN32
    return shared_ptr<UsageReporter>(new WindowsUsageReporter());
#endif
}