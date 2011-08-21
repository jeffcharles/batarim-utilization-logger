#include <string>
#include <Windows.h>

#include "ActiveWindow.hpp"

using std::string;
using std::wstring;

wstring get_wstring_from_tchar(TCHAR* buffer, int buffer_size)
{
    // wstring does not have a constructor that takes a wchar_t, therefore
    // need to copy contents from the wchar_t buffer into a wstring
    wstring ret;
    ret.assign(&buffer[0], &buffer[buffer_size]);
    return ret;
}

wstring get_filename_from_win32_path(wstring& path)
{
    wstring stripped_exe_path;
    bool path_ends_with_exe = path.length() > 4 && 
        path.compare(path.length()-4, 4, L".exe") == 0;
    stripped_exe_path = path_ends_with_exe ? 
        path.substr(0, path.length()-4) : path;
    
    wstring::size_type last_backslash_location = -1;
    if(stripped_exe_path.length() > 1) {
        for(wstring::size_type i = stripped_exe_path.length()-2; i >= 0; --i) {
            if(stripped_exe_path[i] == '\\') {
                last_backslash_location = i;
                break;
            }
        }
    }
    return (last_backslash_location > 0) ? 
        stripped_exe_path.substr(last_backslash_location+1) : 
        stripped_exe_path;
}

ActiveWindow::ActiveWindow()
{
    // Set PID
    HWND handle = GetForegroundWindow();
    DWORD process_id;
    GetWindowThreadProcessId(handle, &process_id);
    pid_ = process_id;

    // Set window name
    const int MAX_WINDOW_NAME_LENGTH = 255;
    TCHAR window_name[MAX_WINDOW_NAME_LENGTH];
    int window_text_length = GetWindowText(
        handle, // handle to window to retrieve name for
        window_name,  // non-const TCHAR string to output to
        MAX_WINDOW_NAME_LENGTH // maximum length of text to copy
    );

    if(window_text_length == 0) {
        name_ = L"";
    } else {
        name_ = get_wstring_from_tchar(window_name, window_text_length);
    }

    // Null out other attributes
    process_name_ = L"";
    cpu_usage_ = 0;
}

wstring ActiveWindow::get_process_name()
{
    if(process_name_ != L"") {
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
        return L"";
    } else {
        process_name_ = get_filename_from_win32_path(
            get_wstring_from_tchar(name, buffer_size));
        return process_name_;
    }
}