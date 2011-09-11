if (WIN32)
    add_library (usage_reporter STATIC
        FormattedProcessCollectionFactory.hpp UsageReporterFactory.hpp
        ProcessUsageInfo.hpp FormattedProcessCollection.hpp
        WindowsFormattedProcessCollection.hpp IUsageResultGetter.hpp
        UsageReporter.hpp WindowsUsageReporter.hpp
        FormattedProcessCollectionFactory.cpp UsageReporterFactory.cpp
        FormattedProcessCollection.cpp UsageReporter.cpp
        WindowsUsageReporter.cpp
    )
    target_link_libraries(usage_reporter pdh raw_process_collection utilities)
elseif (LINUX)
    add_library (usage_reporter STATIC
        FormattedProcessCollectionFactory.hpp UsageReporterFactory.hpp
        ProcessUsageInfo.hpp FormattedProcessCollection.hpp
        LinuxFormattedProcessCollection.hpp IUsageResultGetter.hpp
        UsageReporter.hpp LinuxUsageReporter.hpp
        FormattedProcessCollectionFactory.cpp UsageReporterFactory.cpp
        FormattedProcessCollection.cpp LinuxFormattedProcessCollection.cpp
        UsageReporter.cpp LinuxUsageReporter.cpp
    )
    target_link_libraries (usage_reporter raw_process_collection utilities)
endif ()
