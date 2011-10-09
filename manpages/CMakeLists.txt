SET (MANPAGES
    ${CMAKE_CURRENT_SOURCE_DIR}/batarim-console.1 
    ${CMAKE_CURRENT_SOURCE_DIR}/batarim-logger.1
)
install (FILES ${MANPAGES} DESTINATION man/man1)
