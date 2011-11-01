Set WshShell = CreateObject("WScript.Shell")

installpath = WshShell.RegRead("HKLM\SOFTWARE\Batarim\InstallDir")
logpath = WshShell.RegRead("HKLM\SOFTWARE\Batarim\LogDir")
logfile_postfix = WshShell.RegRead("HKLM\SOFTWARE\Batarim\MajorVersion")

WshShell.Run chr(34) & installpath & "batarim-logger" & chr(34) & " " & chr(34) & logpath & "batarim_" & logfile_postfix & ".csv" & chr(34), 0

Set WshShell = Nothing
