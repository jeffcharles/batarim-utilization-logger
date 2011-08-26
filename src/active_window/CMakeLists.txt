if (WIN32)
    add_library (active_window STATIC 
        active_window_module.hpp IActiveWindow.hpp WindowsActiveWindow.hpp
        windows_active_window_module.cpp WindowsActiveWindow.cpp 
    )
    target_link_libraries (active_window encoding_methods)
elseif (LINUX)
    add_library (active_window STATIC
        active_window_module.hpp IActiveWindow.hpp LinuxActiveWindow.hpp 
        linux_active_window_module.cpp LinuxInvalidActiveWindow.hpp
        LinuxActiveWindow.cpp
    )
    target_link_libraries (active_window X11)
endif ()
