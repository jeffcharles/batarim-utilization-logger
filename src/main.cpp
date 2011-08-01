#include <stdio.h>
#include "cpu_usage.hpp"
#include "ram_usage.hpp"

int main()
{
    int cpu_usage;
    get_cpu_usage(&cpu_usage);
    
    ram_usage_struct ram_usage;
    get_ram_usage(&ram_usage);
    
    printf("Current CPU usage: %d%%\n", cpu_usage);
    printf("Current RAM usage: %d%%\n", ram_usage.percentage_usage);
}
