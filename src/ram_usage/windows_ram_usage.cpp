#include <windows.h>

#include "ram_usage.hpp"

int get_ram_usage()
{
    MEMORYSTATUS memory_status;
    GlobalMemoryStatus(&memory_status);
    return memory_status.dwMemoryLoad;
}
