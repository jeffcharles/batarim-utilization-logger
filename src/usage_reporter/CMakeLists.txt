if (WIN32)
    add_definitions (-DPSAPI_VERSION=1)
    add_library (usage_reporter STATIC
        ProcessList.hpp ProcessInfoOutputs.hpp
        IUsageReporterRequestCollectionSetter.hpp
        UsageReporterRequestCollection.hpp UsageReporter.hpp
        WindowsUsageReporter.hpp usage_reporter_module.hpp
        UsageReporter.cpp WindowsUsageReporter.cpp
        windows_usage_reporter_module.cpp
    )
    target_link_libraries(usage_reporter pdh psapi)
endif ()
