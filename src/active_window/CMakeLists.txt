if (WIN32)
    add_library (active_window STATIC 
        active_window_module.hpp IActiveWindow.hpp WindowsActiveWindow.hpp
        windows_active_window_module.cpp WindowsActiveWindow.cpp 
    )
    target_link_libraries (active_window
        utilities usage_reporter formatted_process_collection
    )
elseif (LINUX)
    add_library (active_window STATIC
        active_window_module.hpp IActiveWindow.hpp LinuxActiveWindow.hpp 
        linux_active_window_module.cpp LinuxInvalidActiveWindow.hpp
        LinuxActiveWindow.cpp
    )
    target_link_libraries (active_window
        X11 utilities usage_reporter formatted_process_collection
    )
endif ()
