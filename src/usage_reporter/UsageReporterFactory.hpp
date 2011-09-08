#ifndef GUARD_UsageReporterFactory_h
#define GUARD_UsageReporterFactory_h

#include <memory>

#include "UsageReporter.hpp"

std::shared_ptr<UsageReporter> get_usage_reporter();

#endif