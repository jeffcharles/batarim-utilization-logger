#include <string>

#include <Windows.h>

#include "WindowsActiveWindow.hpp"

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
}

wstring WindowsActiveWindow::get_process_name()
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
        wstring path = get_wstring_from_tchar(name, buffer_size);
        process_name_ = get_filename_from_win32_path(path);
        return process_name_;
    }
}

int WindowsActiveWindow::get_cpu_usage()
{
    FILETIME idle_time;
    FILETIME creation_time, exit_time;
    FILETIME system_kernel_time1, system_user_time1;
    FILETIME system_kernel_time2, system_user_time2;
    FILETIME process_kernel_time1, process_user_time1;
    FILETIME process_kernel_time2, process_user_time2;

    if(!GetSystemTimes(
        &idle_time,
        &system_kernel_time1,
        &system_user_time1
    )) {
        return -1;
    }

    HANDLE process_handle = OpenProcess(
        PROCESS_QUERY_LIMITED_INFORMATION, // access rights to process
        false, // should processes created by this process inherit the handle
        pid_
    );

    if(!GetProcessTimes(
        process_handle,
        &creation_time,
        &exit_time,
        &process_kernel_time1,
        &process_user_time1
    )) {
        CloseHandle(process_handle);
        return -1;
    }

    CloseHandle(process_handle);

    Sleep(1000);

    if(!GetSystemTimes(
        &idle_time,
        &system_kernel_time2,
        &system_user_time2
    )) {
        return -1;
    }

    process_handle = OpenProcess(
        PROCESS_QUERY_LIMITED_INFORMATION, // access rights to process
        false, // should processes created by this process inherit the handle
        pid_
    );

    if(!GetProcessTimes(
        process_handle,
        &creation_time,
        &exit_time,
        &process_kernel_time2,
        &process_user_time2
    )) {
        CloseHandle(process_handle);
        return -1;
    }

    CloseHandle(process_handle);

    ULONGLONG skt1, skt2, sut1, sut2, pkt1, pkt2, put1, put2;
    skt1 = get_ulonglong_from_filetime(system_kernel_time1);
    skt2 = get_ulonglong_from_filetime(system_kernel_time2);
    sut1 = get_ulonglong_from_filetime(system_user_time1);
    sut2 = get_ulonglong_from_filetime(system_user_time2);
    pkt1 = get_ulonglong_from_filetime(process_kernel_time1);
    pkt2 = get_ulonglong_from_filetime(process_kernel_time2);
    put1 = get_ulonglong_from_filetime(process_user_time1);
    put2 = get_ulonglong_from_filetime(process_user_time2);

    ULONGLONG system_kernel_time_diff = skt2 - skt1;
    ULONGLONG system_user_time_diff = sut2 - sut1;
    ULONGLONG system_time = system_kernel_time_diff + system_user_time_diff;

    ULONGLONG process_kernel_time_diff = pkt2 - pkt1;
    ULONGLONG process_user_time_diff = put2 - put1;
    ULONGLONG process_time = process_kernel_time_diff + process_user_time_diff;

    int cpu_usage = (int)((double)process_time / system_time * 100);

    return cpu_usage;
}

wstring WindowsActiveWindow::get_wstring_from_tchar(TCHAR* buffer, int buffer_size)
{
    // wstring does not have a constructor that takes a wchar_t, therefore
    // need to copy contents from the wchar_t buffer into a wstring

    // FIXME: does not work properly with unicode
    wstring ret;
    ret.assign(&buffer[0], &buffer[buffer_size]);
    return ret;
}

wstring WindowsActiveWindow::get_filename_from_win32_path(wstring& path)
{
    wstring stripped_exe_path;
    bool path_ends_with_exe = path.length() > 4 && 
        path.compare(path.length()-4, 4, L".exe") == 0;
    stripped_exe_path = path_ends_with_exe ? 
        path.substr(0, path.length()-4) : path;
    
    wstring::size_type last_backslash_location = 0;
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

ULONGLONG WindowsActiveWindow::get_ulonglong_from_filetime(FILETIME& ft)
{
    ULARGE_INTEGER uli;
    uli.HighPart = ft.dwHighDateTime;
    uli.LowPart = ft.dwLowDateTime;
    return uli.QuadPart;
}