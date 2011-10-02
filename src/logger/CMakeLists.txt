add_executable (batarim-logger LoggerDisplayer.hpp logger.cpp)
target_link_libraries (batarim-logger batarim)
install (TARGETS batarim-logger
    RUNTIME DESTINATION bin)
