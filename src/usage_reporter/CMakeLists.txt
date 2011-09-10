if (WIN32)
    add_definitions (-DPSAPI_VERSION=1)
    add_library (usage_reporter STATIC
        RawProcessCollectionFactory.hpp FormattedProcessCollectionFactory.hpp
        UsageReporterFactory.hpp RawProcessCollection.hpp
        WindowsRawProcessCollection.hpp ProcessUsageInfo.hpp
        FormattedProcessCollection.hpp WindowsFormattedProcessCollection.hpp
        IUsageResultGetter.hpp UsageReporter.hpp WindowsUsageReporter.hpp
        RawProcessCollectionFactory.cpp FormattedProcessCollectionFactory.cpp
        UsageReporterFactory.cpp RawProcessCollection.cpp
        WindowsRawProcessCollection.cpp FormattedProcessCollection.cpp
        UsageReporter.cpp WindowsUsageReporter.cpp
    )
    target_link_libraries(usage_reporter pdh psapi utilities)
elseif (LINUX)
    add_library (usage_reporter STATIC
        RawProcessCollectionFactory.hpp FormattedProcessCollectionFactory.hpp
        UsageReporterFactory.hpp RawProcessCollection.hpp
        LinuxRawProcessCollection.hpp ProcessUsageInfo.hpp
        FormattedProcessCollection.hpp LinuxFormattedProcessCollection.hpp
        IUsageResultGetter.hpp UsageReporter.hpp LinuxUsageReporter.hpp
        RawProcessCollectionFactory.cpp FormattedProcessCollectionFactory.cpp
        UsageReporterFactory.cpp RawProcessCollection.cpp
        LinuxRawProcessCollection.cpp FormattedProcessCollection.cpp
        LinuxFormattedProcessCollection.cpp UsageReporter.cpp
        LinuxUsageReporter.cpp
    )
    target_link_libraries (usage_reporter utilities)
endif ()
