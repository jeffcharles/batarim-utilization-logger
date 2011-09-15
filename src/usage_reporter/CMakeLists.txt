if (WIN32)
    add_library (usage_reporter STATIC
        IUsageResultGetter.hpp UsageReporter.hpp WindowsUsageReporter.hpp
        UsageReporterFactory.hpp 
        UsageReporterFactory.cpp UsageReporter.cpp WindowsUsageReporter.cpp
    )
    target_link_libraries (usage_reporter
        pdh formatted_process_collection aggregated_process_collection
        process_tree utilities
    )
elseif (LINUX)
    add_library (usage_reporter STATIC
        IUsageResultGetter.hpp UsageReporter.hpp LinuxUsageReporter.hpp
        UsageReporterFactory.hpp
        UsageReporterFactory.cpp UsageReporter.cpp LinuxUsageReporter.cpp
    )
    target_link_libraries (usage_reporter 
        formatted_process_collection aggregated_process_collection process_tree
        utilities
    )
endif ()
