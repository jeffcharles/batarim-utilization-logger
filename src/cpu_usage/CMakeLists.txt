if (WIN32)
    add_library (cpu_usage STATIC 
        cpu_usage.hpp windows_pdh_exception.hpp windows_cpu_usage.cpp)
    target_link_libraries (cpu_usage pdh)
elseif (LINUX)
    add_library (cpu_usage STATIC cpu_usage.hpp linux_cpu_usage.cpp)
endif ()
