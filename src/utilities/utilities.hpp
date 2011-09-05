#ifndef GUARD_utilities_h
#define GUARD_utilities_h

#include <string>

namespace batarim {
    std::string get_filename_from_path(std::string& path);
    std::string get_process_name(unsigned int pid);
}

#endif