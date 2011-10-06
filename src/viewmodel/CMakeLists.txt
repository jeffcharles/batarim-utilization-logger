add_library (viewmodel SHARED
    DLLDefines.hpp viewmodel.hpp IDisplayer.hpp viewmodel.cpp
)
target_link_libraries (viewmodel
    usage_reporter formatted_process_collection ram_usage active_window
    timestamp uptime
)
