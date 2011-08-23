if (WIN32)
    add_library (active_window STATIC 
        IActiveWindow.hpp WindowsActiveWindow.hpp WindowsActiveWindow.cpp 
    )
elseif (LINUX)
    add_library (active_window STATIC
        active_window_module.hpp IActiveWindow.hpp LinuxActiveWindow.hpp 
        linux_active_window_module.cpp LinuxInvalidActiveWindow.hpp
        LinuxActiveWindow.cpp
    )
    target_link_libraries (active_window X11)
endif ()
