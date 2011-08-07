#include <windows.h>

#include "ram_usage.hpp"

int get_ram_usage()
{
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    int method_success = GlobalMemoryStatusEx(&statex);
    return method_success ? (int)statex.dwMemoryLoad : -1;
}
