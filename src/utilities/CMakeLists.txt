if (WIN32)
    add_library (encoding_methods STATIC
        windows_encoding_methods.hpp windows_encoding_methods.cpp
    )
endif ()
