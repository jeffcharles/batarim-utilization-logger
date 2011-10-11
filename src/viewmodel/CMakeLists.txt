add_library (viewmodel SHARED
    DLLDefines.hpp viewmodel.hpp IDisplayer.hpp viewmodel.cpp
)
set_target_properties (viewmodel
    PROPERTIES OUTPUT_NAME batarim
)
target_link_libraries (viewmodel
    usage_reporter formatted_process_collection ram_usage active_window
    timestamp uptime
)
if (NOT WIN32)
    install (TARGETS viewmodel
        LIBRARY DESTINATION lib)
else ()
    install (TARGETS viewmodel
        RUNTIME DESTINATION .)
endif ()
