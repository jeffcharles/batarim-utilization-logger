# RAM usage library
if (WIN32) 
    add_library (ram_usage STATIC ram_usage.hpp windows_ram_usage.cpp)
else ()
    add_library (ram_usage STATIC ram_usage.hpp unix_ram_usage.cpp)
endif ()

# Main executable
add_executable (main main.cpp)
target_link_libraries (main ram_usage)
