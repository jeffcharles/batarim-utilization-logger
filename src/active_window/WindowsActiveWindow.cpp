#include <string>

#include <Windows.h>

#include "../utilities/windows_encoding_methods.hpp"

#include "WindowsActiveWindow.hpp"

using batarim::encoding_methods::convert_wstring_to_string;
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

void WindowsActiveWindow::populate_cpu_usage(
    IUsageReporterRequestCollectionSetter& request_collection,
    int* cpu_usage
) {
    request_collection.add_specific_process_cpu_request(pid_, cpu_usage);
}

string WindowsActiveWindow::get_filename_from_win32_path(string& path)
{
    string stripped_exe_path;
    bool path_ends_with_exe = path.length() > 4 && 
        path.compare(path.length()-4, 4, ".exe") == 0;
    stripped_exe_path = path_ends_with_exe ? 
        path.substr(0, path.length()-4) : path;
    
    string::size_type last_backslash_location = 0;
    if(stripped_exe_path.length() > 1) {
        for(string::size_type i = stripped_exe_path.length()-2; i >= 0; --i) {
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