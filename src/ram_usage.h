#ifndef GUARD_ram_usage_h
#define GUARD_ram_usage_h

typedef struct {
    int percentage_usage;
} ram_usage_struct;

void get_ram_usage(ram_usage_struct*);

#endif
