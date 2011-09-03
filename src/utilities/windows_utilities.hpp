#ifndef GUARD_encoding_methods_h
#define GUARD_encoding_methods_h

#include <string>

namespace batarim {
    std::string convert_wstring_to_string(std::wstring wide_str);
    std::string get_filename_from_win32_path(std::string& path);
}

#endif