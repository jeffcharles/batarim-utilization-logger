#include <memory>
#include <string>

#include <time.h>

struct TimeInfo
{
    time_t timestamp;
    std::string datetime_str;
};

std::shared_ptr<TimeInfo> get_time_info();