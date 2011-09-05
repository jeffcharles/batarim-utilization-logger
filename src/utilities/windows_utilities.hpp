#ifndef GUARD_windows_utilities_h
#define GUARD_windows_utilities_h

#include <string>

namespace batarim {
    std::string convert_wstring_to_string(std::wstring wide_str);
}

#endif