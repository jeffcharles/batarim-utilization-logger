add_executable (console ConsoleDisplayer.hpp console.cpp)
set_target_properties (console
    PROPERTIES OUTPUT_NAME batarim-console
)
target_link_libraries (console viewmodel)
install (TARGETS console
    RUNTIME DESTINATION bin)
