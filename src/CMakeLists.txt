add_subdirectory (utilities)
add_subdirectory (raw_process_collection)
add_subdirectory (formatted_process_collection)
add_subdirectory (aggregated_process_collection)
add_subdirectory (process_tree)
add_subdirectory (usage_reporter)
add_subdirectory(ram_usage)
add_subdirectory(active_window)

# Main executable
add_executable (main main.cpp)
target_link_libraries (main
    usage_reporter formatted_process_collection ram_usage active_window
)
