if (WIN32)
    add_library (utilities STATIC
        utilities.hpp windows_utilities.hpp windows_utilities.cpp
    )
elseif (LINUX)
    add_library (utilities STATIC
        utilities.hpp linux_utilities.cpp
    )
endif ()
