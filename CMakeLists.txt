cmake_minimum_required (VERSION 2.8)
project (BatarimUL)

# LINUX is true if compiling on Linux else it is false
SET (LINUX "${CMAKE_SYSTEM_NAME}" MATCHES "Linux")

if (NOT WIN32 AND NOT LINUX)
    message (FATAL_ERROR 
"Platform not supported. 
Only Windows and Linux are currently supported."
    )
endif ()

# FIXME: below should check if using g++, not if on Linux
if (LINUX)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
endif ()

add_subdirectory (src)
