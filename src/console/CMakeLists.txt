add_executable (batarim-console ConsoleDisplayer.hpp console.cpp)
target_link_libraries (batarim-console batarim)
install (TARGETS batarim-console
    RUNTIME DESTINATION bin)
