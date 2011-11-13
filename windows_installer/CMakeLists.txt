if (CMAKE_CL_64)
    set (candle_arch x64)
else ()
    set (candle_arch x86)
endif ()
    
set (candle_cmd
    "\\Windows Installer XML v3.5\\bin\\candle\" -arch ${candle_arch} -dPlatform=${candle_arch} -dVersion=${VERSION} -dLogFilePostfix=${LOG_FILENAME_POSTFIX} *.wxs")

set (light_cmd
    "\\Windows Installer XML v3.5\\bin\\light\" -ext WixUIExtension -ext WixUtilExtension -cultures:en-us -loc \"${CMAKE_CURRENT_SOURCE_DIR}\\Batarim_en-us.wxl\" -out batarim_${VERSION}_${candle_arch}.msi *.wixobj")

add_custom_command (
    OUTPUT ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}\\batarim_${VERSION}_${candle_arch}.msi
    
    COMMAND if $<CONFIGURATION>==Release copy \"${CMAKE_CURRENT_SOURCE_DIR}\\*.wxs\" Release\\
    COMMAND if $<CONFIGURATION>==Release copy \"${CMAKE_CURRENT_SOURCE_DIR}\\task.vbs\" Release\\
    COMMAND if $<CONFIGURATION>==Release copy \"${CMAKE_SOURCE_DIR}\\batarim.ico\" Release\\
    COMMAND if $<CONFIGURATION>==Release copy \"${CMAKE_CURRENT_SOURCE_DIR}\\license.rtf\" Release\\
    COMMAND if $<CONFIGURATION>==Release copy \"${CMAKE_CURRENT_SOURCE_DIR}\\top_banner.bmp\" Release\\
    COMMAND if $<CONFIGURATION>==Release copy \"${CMAKE_CURRENT_SOURCE_DIR}\\dlg_bkg.bmp\" Release\\
    COMMAND cmd /c "\"if $<CONFIGURATION>==Release if defined PROGRAMFILES(X86) (cd Release & \"%PROGRAMFILES(X86)%${candle_cmd} & cd ..)\""
    COMMAND cmd /c "\"if $<CONFIGURATION>==Release if not defined PROGRAMFILES(X86) (cd Release & \"%PROGRAMFILES%${candle_cmd} & cd ..)\""
    COMMAND cmd /c "\"if $<CONFIGURATION>==Release if defined PROGRAMFILES(X86) (cd Release & \"%PROGRAMFILES(X86)%${light_cmd} & cd ..)\""
    COMMAND cmd /c "\"if $<CONFIGURATION>==Release if not defined PROGRAMFILES(X86) (cd Release & \"%PROGRAMFILES%${light_cmd} & cd ..)\""
    
    DEPENDS viewmodel console logger
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

add_custom_target (installer ALL
    DEPENDS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}\\batarim_${VERSION}_${candle_arch}.msi
)
