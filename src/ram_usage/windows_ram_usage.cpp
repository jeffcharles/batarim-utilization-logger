#include <iostream>
#include <windows.h>

#include "ram_usage.hpp"

using std::cerr;
using std::clog;
using std::endl;
using std::hex;

int get_ram_usage()
{
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    int method_success = GlobalMemoryStatusEx(&statex);
    if(method_success) {
        return (int)statex.dwMemoryLoad;
    } else {
        cerr << "Could not determine memory status. Error code = 0x" << hex 
            << method_success << endl;
        clog << "Could not determine memory status. Error code = 0x" << hex 
            << method_success << endl;
        return -1;
    }
}
