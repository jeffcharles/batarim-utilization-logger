add_subdirectory (utilities)
add_subdirectory (usage_reporter)

add_subdirectory(cpu_usage)
add_subdirectory(ram_usage)
add_subdirectory(active_window)

# Main executable
add_executable (main main.cpp)
target_link_libraries (main usage_reporter cpu_usage ram_usage active_window)
