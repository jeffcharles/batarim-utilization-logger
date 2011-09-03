if (WIN32)
    add_definitions (-DPSAPI_VERSION=1)
    add_library (usage_reporter STATIC
        ProcessList.hpp ProcessUsageInfo.hpp IUsageResultGetter.hpp
        UsageReporter.hpp WindowsUsageReporter.hpp usage_reporter_module.hpp
        ProcessList.cpp UsageReporter.cpp WindowsUsageReporter.cpp
        windows_usage_reporter_module.cpp
    )
    target_link_libraries(usage_reporter pdh psapi utilities)
elseif (LINUX)
    add_library (usage_reporter STATIC
        ProcessList.hpp ProcessUsageInfo.hpp IUsageResultGetter.hpp
        UsageReporter.hpp LinuxUsageReporter.hpp usage_reporter_module.hpp
        ProcessList.cpp UsageReporter.cpp LinuxUsageReporter.cpp
        linux_usage_reporter_module.cpp
    )
endif ()
