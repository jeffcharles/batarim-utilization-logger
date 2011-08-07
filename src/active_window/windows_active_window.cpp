#include <string>
#include <Windows.h>

#include "active_window.hpp"

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

wstring get_active_window_name()
{
    HWND active_window = GetForegroundWindow();
    const int MAX_WINDOW_NAME_LENGTH = 255;
    TCHAR window_name[MAX_WINDOW_NAME_LENGTH];
    int window_text_retrieved_length = GetWindowText(
        active_window, // handle to window to retrieve name for
        window_name,  // non-const TCHAR string to output to
        MAX_WINDOW_NAME_LENGTH // maximum length of text to copy
    );

    if(window_text_retrieved_length == 0) {
        return L"";
    } else {
        return get_wstring_from_tchar(window_name, 
            window_text_retrieved_length);
    }
}

wstring get_filename_from_win32_path(wstring& path)
{
    wstring stripped_exe_path;
    bool path_ends_with_exe = path.length() >= 4 && 
        path.compare(path.length()-5, 4, L".exe") == 0;
    stripped_exe_path = path_ends_with_exe ? 
        path.substr(0, path.length()-5) : path;
    
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

wstring get_active_window_process_name()
{
    HWND active_window = GetForegroundWindow();
    DWORD process_id;
    GetWindowThreadProcessId(active_window, &process_id);
    HANDLE process_handle = OpenProcess(
        PROCESS_QUERY_LIMITED_INFORMATION, // access rights to process
        false, // should processes created by this process inherit the handle
        process_id
    );
    
    const int MAX_PROCESS_NAME_LENGTH = 255;
    TCHAR process_name[MAX_PROCESS_NAME_LENGTH];
    DWORD buffer_size = MAX_PROCESS_NAME_LENGTH;
    
    bool query_success = QueryFullProcessImageName(
        process_handle,
        NULL, // use Win32 path format
        process_name,
        &buffer_size
    ) != 0;

    CloseHandle(process_handle);

    if(!query_success || buffer_size == 0) {
        return L"";
    } else {
        return get_filename_from_win32_path(
            get_wstring_from_tchar(process_name, buffer_size));
    }
}
