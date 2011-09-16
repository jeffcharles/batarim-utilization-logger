if (LINUX)
    add_library (uptime STATIC uptime.hpp linux_uptime.cpp)
endif ()
