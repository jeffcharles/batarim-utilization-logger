#ifndef GUARD_cpu_usage_h
#define GUARD_cpu_usage_h

#include <string>
#include <vector>

// Returns a list of processor percent utilizations
void get_cpu_usage(std::vector<std::pair<std::string, int> >&);

#endif
