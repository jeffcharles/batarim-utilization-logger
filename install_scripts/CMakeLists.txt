install (CODE "
    execute_process (
        COMMAND cp \"${CMAKE_CURRENT_SOURCE_DIR}/setup.sh\" \"${CMAKE_CURRENT_BINARY_DIR}/\"
    )

    execute_process (
        COMMAND sed -i \"s/<_postfix_>/${LOG_FILENAME_POSTFIX}/\" \"${CMAKE_CURRENT_BINARY_DIR}/setup.sh\"
    )

    execute_process (
        COMMAND \"${CMAKE_CURRENT_BINARY_DIR}/setup.sh\" ${LOG_FILENAME_POSTFIX}
        RESULT_VARIABLE rv
    )
    
    if (NOT rv EQUAL 0)
        message (\"\${rv}\")
    endif ()
")
