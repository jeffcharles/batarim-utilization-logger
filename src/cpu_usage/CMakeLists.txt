if (WIN32)
    add_library (cpu_usage STATIC 
        cpu_usage.hpp windows_exception.hpp 
        windows_com_setup_exception.hpp windows_wmi_exception.hpp 
        windows_cpu_usage.cpp
    )
    target_link_libraries (cpu_usage comsuppw)
else ()
    add_library (cpu_usage STATIC cpu_usage.hpp unix_cpu_usage.cpp)
endif ()
