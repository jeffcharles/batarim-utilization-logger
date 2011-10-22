set (wxs_files batarim.wxs Batarim_LogDirDlg.wxs BatarimUI_Workflow.wxs)

if (CMAKE_CL_64)
    set (candle_arch x64)
else ()
    set (candle_arch x86)
endif ()

FILE (COPY DESTINATION . FILES_MATCHING PATTERN "*.wxs") 
FILE (COPY Batarim_en-us.wxl DESTINATION .)
FILE (COPY License.rtf DESTINATION .)

foreach (wxs_file ${wxs_files})
    string (REPLACE ".wxs" ".wixobj" obj_file ${wxs_file})
    
    set (candle_cmd
        "\\Windows Installer XML v3.5\\bin\\candle\" -arch ${candle_arch} -dPlatform=${candle_arch} -o ${obj_file} ${wxs_file}")

    FILE (COPY ${wxs_file} DESTINATION .)

    add_custom_command (
        OUTPUT ${obj_file}

        COMMAND cmd /c "if defined PROGRAMFILES(X86) (\"%PROGRAMFILES(X86)%${candle_cmd}\) else \(\"%PROGRAMFILES%${candle_cmd}\)"
        
        DEPENDS organize

        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
    
    list (APPEND obj_files ${obj_file})
endforeach()

foreach (obj ${obj_files})
    set(obj_files_str "${obj_files_str} ${obj}")
endforeach ()

set (light_cmd
    "\\Windows Installer XML v3.5\\bin\\light\" -ext WixUIExtension -ext WixUtilExtension -cultures:en-us -loc Batarim_en-us.wxl -out batarim.msi ${obj_files_str}")

message (STATUS "if defined PROGRAMFILES(X86) (\"%PROGRAMFILES(X86)%${light_cmd}) else (\"%PROGRAMFILES%${light_cmd})")

add_custom_target (installer ALL
    
    COMMAND cmd /c "if defined PROGRAMFILES(X86) (\"%PROGRAMFILES(X86)%${light_cmd}) else (\"%PROGRAMFILES%${light_cmd})"
    
    DEPENDS ${obj_files}

    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
