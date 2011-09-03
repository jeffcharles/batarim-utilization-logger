if (WIN32)
    add_library (utilities STATIC
        windows_utilities.hpp windows_utilities.cpp
    )
elseif (LINUX)
    add_library (utilities STATIC
        linux_utilities.hpp linux_utilities.cpp
    )
endif ()
