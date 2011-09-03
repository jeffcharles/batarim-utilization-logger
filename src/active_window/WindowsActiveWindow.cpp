#include <string>

#include <Windows.h>

#include "../utilities/windows_utilities.hpp"

#include "WindowsActiveWindow.hpp"

using batarim::convert_wstring_to_string;
using batarim::get_filename_from_win32_path;
using std::string;
using std::wstring;

WindowsActiveWindow::WindowsActiveWindow()
{
    // Set PID
    HWND handle = GetForegroundWindow();
    DWORD process_id;
    GetWindowThreadProcessId(handle, &process_id);
    pid_ = process_id;

    // Set window name
    const int MAX_WINDOW_NAME_LENGTH = 255;
    WCHAR window_name[MAX_WINDOW_NAME_LENGTH];
    int window_text_length = GetWindowText(
        handle, // handle to window to retrieve name for
        window_name,  // non-const TCHAR string to output to
        MAX_WINDOW_NAME_LENGTH // maximum length of text to copy
    );

    if(window_text_length == 0) {
        name_ = "";
    } else {
        name_ = convert_wstring_to_string(window_name);
    }

    // Null out other attributes
    process_name_ = "";
}

string WindowsActiveWindow::get_process_name()
{
    if(process_name_ != "") {
        return process_name_;
    }

    HANDLE process_handle = OpenProcess(
        PROCESS_QUERY_LIMITED_INFORMATION, // access rights to process
        false, // should processes created by this process inherit the handle
        pid_
    );
    
    const int MAX_PROCESS_NAME_LENGTH = 255;
    TCHAR name[MAX_PROCESS_NAME_LENGTH];
    DWORD buffer_size = MAX_PROCESS_NAME_LENGTH;
    
    bool query_success = QueryFullProcessImageName(
        process_handle,
        NULL, // use Win32 path format
        name,
        &buffer_size
    ) != 0;

    CloseHandle(process_handle);

    if(!query_success || buffer_size == 0) {
        return "";
    } else {
        string path = convert_wstring_to_string(name);
        process_name_ = get_filename_from_win32_path(path);
        return process_name_;
    }
}