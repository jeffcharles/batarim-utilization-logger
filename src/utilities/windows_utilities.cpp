#include <memory>
#include <string>

#include <Windows.h>

using std::string;
using std::unique_ptr;
using std::wstring;

namespace batarim {
        
    string convert_wstring_to_string(wstring wide_str)
    {
        // Run WideCharToMultiByte once with NULL output buffer to get
        // required size of output buffer. Run it a second time with a
        // non-NULL output buffer to populate the output buffer.
        // See
        // http://msdn.microsoft.com/en-us/library/dd374130(v=vs.85).aspx
        // for details.
        const int buffer_size = WideCharToMultiByte(
            CP_UTF8, // encoding to convert to (i.e., UTF-16 to UTF-8)
            0, // conversion flags (none look necessary)
            wide_str.c_str(), // UTF-16 buffer to convert from
            wide_str.length()+1, // size of UTF-16 buffer
            NULL, // buffer receiving UTF-8 string
            0, // size of UTF-8 buffer (0 means calculate required size)
            NULL, // default character (must be NULL for UTF-8)
            NULL // ptr to indicate if default char used (NULL for UTF-8)
        );
        unique_ptr<char> multibyte_buffer(new char[buffer_size]);
        WideCharToMultiByte(
            CP_UTF8,
            0,
            wide_str.c_str(),
            wide_str.length()+1,
            multibyte_buffer.get(), // buffer receiving UTF-8 string
            buffer_size, // size of UTF-8 buffer
            NULL,
            NULL
        );
        return string(multibyte_buffer.get());
    }

    string get_filename_from_win32_path(string& path)
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

    std::string get_process_name(unsigned int pid)
    {
        HANDLE process_handle = OpenProcess(
            PROCESS_QUERY_LIMITED_INFORMATION, // access rights
            false, // child processes should not inherit handle
            pid
        );

        const int MAX_PROCESS_NAME_LENGTH = 255;
        WCHAR name[MAX_PROCESS_NAME_LENGTH];
        DWORD buffer_size = MAX_PROCESS_NAME_LENGTH;
    
        bool query_success = QueryFullProcessImageName(
            process_handle,
            NULL, // use Win32 path format
            name,
            &buffer_size
        ) != 0;

        CloseHandle(process_handle);

        string process_path = convert_wstring_to_string(name);
        string process_name = get_filename_from_win32_path(process_path);

        return process_name;
    }
}