#include <memory>

#include "WindowsUsageReporter.hpp"

#include "usage_reporter_module.hpp"

using std::shared_ptr;

shared_ptr<UsageReporter> get_usage_reporter()
{
    shared_ptr<UsageReporter> usage_reporter(new WindowsUsageReporter());
    return usage_reporter;
}