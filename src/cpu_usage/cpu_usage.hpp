#ifndef GUARD_cpu_usage_h
#define GUARD_cpu_usage_h

#include <string>
#include <vector>

#include "../usage_reporter/IUsageReporterRequestCollectionSetter.hpp"

// Returns a list of processor percent utilizations
void get_cpu_usage(
    IUsageReporterRequestCollectionSetter& request_collection,
    std::vector<std::pair<std::string, int> >&
);

#endif
