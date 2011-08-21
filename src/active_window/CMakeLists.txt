if (WIN32)
    add_library (active_window STATIC 
        ActiveWindow.hpp windows_ActiveWindow.cpp
    )
elseif (LINUX)
    add_library (active_window STATIC
        ActiveWindow.hpp linux_ActiveWindow.cpp
    )
    target_link_libraries (active_window X11)
endif ()
