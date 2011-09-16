#include <Windows.h>

#include "uptime.hpp"

unsigned int get_uptime()
{
    return GetTickCount64() / 1000;
}