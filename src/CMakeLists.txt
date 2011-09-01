add_subdirectory (utilities)
add_subdirectory (usage_reporter)

if (LINUX)
    add_subdirectory(cpu_usage)
endif ()
add_subdirectory(ram_usage)
add_subdirectory(active_window)

# Main executable
add_executable (main main.cpp)
if (WIN32)
    target_link_libraries (main usage_reporter ram_usage active_window)
elseif (LINUX)
    target_link_libraries (main
        usage_reporter cpu_usage ram_usage active_window
    )
endif ()
