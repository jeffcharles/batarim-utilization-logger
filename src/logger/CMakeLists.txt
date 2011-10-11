add_executable (logger LoggerDisplayer.hpp logger.cpp)
set_target_properties (logger
    PROPERTIES OUTPUT_NAME batarim-logger
)
target_link_libraries (logger viewmodel)

install (TARGETS logger
    RUNTIME DESTINATION bin)
