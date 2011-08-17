if (WIN32)
    add_library (active_window STATIC 
        active_window.hpp windows_active_window.cpp
    )
elseif (LINUX)
    add_library (active_window STATIC
        active_window.hpp linux_active_window.cpp
    )
endif ()
