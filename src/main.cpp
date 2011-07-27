#include <stdio.h>
#include "ram_usage.hpp"

int main()
{
    ram_usage_struct ram_usage;
    get_ram_usage(&ram_usage);
    printf("Current RAM usage: %d%%\n", ram_usage.percentage_usage);
}
