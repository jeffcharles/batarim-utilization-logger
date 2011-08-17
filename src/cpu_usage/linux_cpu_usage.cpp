#include <string>
#include <vector>

#include "cpu_usage.hpp"

using std::pair;
using std::vector;
using std::wstring;

// TODO: make this calculate CPU usage
void get_cpu_usage(vector<pair<wstring, int> >& usage_percentages)
{
    pair<wstring, int> usage_percentage(L"1", 101);
    usage_percentages.push_back(usage_percentage);
}
