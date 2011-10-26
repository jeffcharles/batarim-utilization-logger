#include <Windows.h>

#include "uptime.hpp"

unsigned int get_uptime()
{
#pragma warning(disable: 4244)
    // 4,294,967,295 seconds
    // / 60 sec/min / 60 min/hr / 24 hr/day / ~365 day/yr = ~136 years
    // No one who is running this software should have an uptime higher than
    // 136 years, so disregard the possible loss of data warning
    return GetTickCount64() / 1000;
#pragma warning(default: 4244)
}