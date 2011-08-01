# CPU usage library
if (WIN32)
    add_library (cpu_usage STATIC 
        cpu_usage.hpp windows_exception.hpp 
        windows_com_setup_exception.hpp windows_wmi_exception.hpp 
        windows_cpu_usage.cpp
    )
    target_link_libraries (cpu_usage comsuppw)
else ()
    add_library (cpu_usage STATIC cpu_usage.hpp unix_cpu_usage.cpp)
endif ()

# RAM usage library
if (WIN32) 
    add_library (ram_usage STATIC ram_usage.hpp windows_ram_usage.cpp)
else ()
    add_library (ram_usage STATIC ram_usage.hpp unix_ram_usage.cpp)
endif ()

# Main executable
add_executable (main main.cpp)
target_link_libraries (main cpu_usage ram_usage)
