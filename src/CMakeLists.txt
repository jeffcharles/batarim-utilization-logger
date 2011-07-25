# RAM usage library
if (WIN32) 
    add_library (ram_usage STATIC ram_usage.h windows_ram_usage.c)
else ()
    add_library (ram_usage STATIC ram_usage.h unix_ram_usage.c)
endif ()

# Main executable
add_executable (main main.c)
target_link_libraries (main ram_usage)
