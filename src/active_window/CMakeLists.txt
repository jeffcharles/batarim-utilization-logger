if (WIN32)
    add_library (active_window STATIC 
        active_window.hpp windows_active_window.cpp
    )
else ()
    add_library (active_window STATIC
        active_window.hpp unix_active_window.cpp
    )
endif ()
