add_subdirectory(cpu_usage)
add_subdirectory(ram_usage)

# Main executable
add_executable (main main.cpp)
target_link_libraries (main cpu_usage ram_usage)
