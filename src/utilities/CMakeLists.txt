if (WIN32)
    add_library (utilities STATIC
        windows_utilities.hpp windows_utilities.cpp
    )
endif ()
