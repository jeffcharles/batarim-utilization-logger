#ifndef GUARD_usage_reporter_module_h
#define GUARD_usage_reporter_module_h

#include <memory>

#include "UsageReporter.hpp"

std::shared_ptr<UsageReporter> get_usage_reporter();

#endif