install (CODE "
    execute_process (
        COMMAND \"${CMAKE_CURRENT_SOURCE_DIR}/setup.sh\" ${LOG_FILENAME_POSTFIX}
        RESULT_VARIABLE rv
    )
    
    if (NOT rv EQUAL 0)
        message (\"\${rv}\")
    endif ()
")
