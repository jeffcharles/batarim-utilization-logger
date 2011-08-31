#include <string>
#include <vector>

#include "../usage_reporter/IUsageReporterRequestCollectionSetter.hpp"

#include "cpu_usage.hpp"

using std::pair;
using std::string;
using std::vector;

void get_cpu_usage(
    IUsageReporterRequestCollectionSetter& request_collection,
    vector<pair<string, int> >& usage_percentages
) {
    request_collection.add_procesors_usage_request(&usage_percentages);
}
