#include <windows.h>
#include "ram_usage.hpp"

void get_ram_usage(ram_usage_struct* ram_usage)
{
    MEMORYSTATUSEX statex;
    int method_success;

    statex.dwLength = sizeof(statex);
	
    method_success = GlobalMemoryStatusEx(&statex);
    if(method_success) {
        ram_usage->percentage_usage = (int)statex.dwMemoryLoad;
    }
}
