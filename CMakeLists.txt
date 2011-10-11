cmake_minimum_required (VERSION 2.8)
project (BatarimUL)

# LINUX is true if compiling on Linux else it is false
# Originally this used a set command against the below but that set LINUX to a
# non-false value on Windows systems
if ("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
    set (LINUX TRUE)
    add_definitions (-DLINUX)
else ()
    set (LINUX FALSE)
endif ()

# This is a necessary sanity check since it has failed in the past
if (WIN32 AND LINUX)
    message (FATAL_ERROR
"Something is wrong with the base CMakeLists.txt file.
WIN32 and LINUX should not both be true."
    )
endif ()

if (NOT WIN32 AND NOT LINUX)
    message (FATAL_ERROR 
"Platform not supported. 
Only Windows and Linux are currently supported."
    )
endif ()

if (MSVC)
    # Use unicode instead of multi-byte (i.e., ANSI)
    add_definitions (-DUNICODE -D_UNICODE)
    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /W4")
endif ()

if(CMAKE_COMPILER_IS_GNUCXX)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
endif ()

add_subdirectory (src)
add_subdirectory (manpages)
add_subdirectory (install_scripts)

# uninstall target
configure_file (
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY
)
add_custom_target (uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/install_scripts/teardown.sh
)

# Copy all built files into own directory on Windows
if (WIN32)
    add_custom_target (ORGANIZE ALL
        COMMAND IF NOT EXIST \"${CMAKE_BINARY_DIR}\\dist\\bin\" MKDIR \"${CMAKE_BINARY_DIR}\\dist\\bin\" > nul
        COMMAND FOR /F \"tokens=*\" %%A IN ('DIR /B /S *.dll') DO (COPY \"%%A\" \"${CMAKE_BINARY_DIR}\\dist\\bin\") > nul
        COMMAND FOR /F \"tokens=*\" %%A IN ('DIR /B /S *.exe') DO (COPY \"%%A\" \"${CMAKE_BINARY_DIR}\\dist\\bin\") > nul
        DEPENDS viewmodel console logger
    )
endif ()
