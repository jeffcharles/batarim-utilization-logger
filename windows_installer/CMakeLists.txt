if (CMAKE_CL_64)
    set (candle_arch x64)
else ()
    set (candle_arch x86)
endif ()
    
set (candle_cmd
    "\\Windows Installer XML v3.5\\bin\\candle\" -arch ${candle_arch} -dPlatform=${candle_arch} -dVersion=${VERSION} *.wxs")

set (light_cmd
    "\\Windows Installer XML v3.5\\bin\\light\" -ext WixUIExtension -ext WixUtilExtension -cultures:en-us -loc \"${CMAKE_CURRENT_SOURCE_DIR}\\Batarim_en-us.wxl\" -out batarim_${VERSION}_${candle_arch}.msi *.wixobj")

add_custom_target (installer ALL
    COMMAND copy \"${CMAKE_CURRENT_SOURCE_DIR}\\*.wxs\" .
    COMMAND copy \"${CMAKE_CURRENT_SOURCE_DIR}\\license.rtf\" .
    COMMAND copy \"${CMAKE_CURRENT_SOURCE_DIR}\\top_banner.bmp\" .
    COMMAND copy \"${CMAKE_CURRENT_SOURCE_DIR}\\dlg_bkg.bmp\" .
    COMMAND cmd /c "if defined PROGRAMFILES(X86) (\"%PROGRAMFILES(X86)%${candle_cmd}\) else \(\"%PROGRAMFILES%${candle_cmd}\)"
    COMMAND cmd /c "if defined PROGRAMFILES(X86) (\"%PROGRAMFILES(X86)%${light_cmd}) else (\"%PROGRAMFILES%${light_cmd})"
    
    DEPENDS organize
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
