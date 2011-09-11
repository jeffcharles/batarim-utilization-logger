if (WIN32)
    add_library (formatted_process_collection STATIC
        ProcessUsageInfo.hpp FormattedProcessCollection.hpp
        WindowsFormattedProcessCollection.hpp
        FormattedProcessCollectionFactory.hpp
        FormattedProcessCollection.cpp FormattedProcessCollectionFactory.cpp
    )
elseif (LINUX)
    add_library (formatted_process_collection STATIC
        ProcessUsageInfo.hpp FormattedProcessCollection.hpp
        LinuxFormattedProcessCollection.hpp
        FormattedProcessCollectionFactory.hpp
        FormattedProcessCollection.cpp LinuxFormattedProcessCollection.cpp
        FormattedProcessCollectionFactory.cpp
    )
endif ()

target_link_libraries (formatted_process_collection
    raw_process_collection utilities
)
