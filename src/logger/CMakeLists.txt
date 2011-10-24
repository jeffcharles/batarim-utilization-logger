if (WIN32)
    add_executable (logger LoggerDisplayer.hpp logger.cpp batarim.rc)
else ()
    add_executable (logger LoggerDisplayer.hpp logger.cpp)
endif ()
set_target_properties (logger
    PROPERTIES OUTPUT_NAME batarim-logger
)
target_link_libraries (logger viewmodel)

install (TARGETS logger
    RUNTIME DESTINATION bin)
