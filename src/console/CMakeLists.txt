if (WIN32)
    add_executable (console ConsoleDisplayer.hpp console.cpp batarim.rc)
else ()
    add_executable (console ConsoleDisplayer.hpp console.cpp)
endif ()
set_target_properties (console
    PROPERTIES OUTPUT_NAME batarim-console
)
target_link_libraries (console viewmodel)
install (TARGETS console
    RUNTIME DESTINATION bin)
