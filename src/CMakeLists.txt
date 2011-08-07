add_subdirectory(cpu_usage)
add_subdirectory(ram_usage)
add_subdirectory(active_window)

# Main executable
add_executable (main main.cpp)
target_link_libraries (main cpu_usage ram_usage active_window)
