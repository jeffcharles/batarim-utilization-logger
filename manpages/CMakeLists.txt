SET (MANPAGES
    ${CMAKE_CURRENT_BINARY_DIR}/batarim-console.1.gz
    ${CMAKE_CURRENT_BINARY_DIR}/batarim-logger.1.gz
)
add_custom_target (manpages ALL
    COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/*.1" "${CMAKE_CURRENT_BINARY_DIR}/"
    COMMAND gzip -f ${CMAKE_CURRENT_BINARY_DIR}/*.1
)
install (FILES ${MANPAGES} DESTINATION man/man1)
