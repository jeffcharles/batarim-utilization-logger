add_library (batarim SHARED
    DLLDefines.hpp viewmodel.hpp IDisplayer.hpp viewmodel.cpp
)
target_link_libraries (batarim
    usage_reporter formatted_process_collection ram_usage active_window
    timestamp uptime
)
install (TARGETS batarim
    LIBRARY DESTINATION lib)
