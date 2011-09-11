if (WIN32)
    add_definitions (-DPSAPI_VERSION=1)
    add_library (raw_process_collection STATIC
        RawProcessCollection.hpp WindowsRawProcessCollection.hpp
        RawProcessCollectionFactory.hpp
        RawProcessCollection.cpp WindowsRawProcessCollection.cpp
        RawProcessCollectionFactory.cpp
    )
    target_link_libraries (raw_process_collection psapi)
elseif (LINUX)
    add_library (raw_process_collection STATIC
        RawProcessCollection.hpp LinuxRawProcessCollection.hpp
        RawProcessCollectionFactory.hpp
        RawProcessCollection.cpp LinuxRawProcessCollection.cpp
        RawProcessCollectionFactory.cpp
    )
endif ()
