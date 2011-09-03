#include <cstring>
#include <memory>
#include <string>

#include "linux_utilities.hpp"

using std::string;
using std::unique_ptr;

namespace batarim {

    string get_filename_from_path(string path)
    {
        const char* const_c_path = path.c_str();
        unique_ptr<char> c_path(new char[strlen(const_c_path)+1]);
        strcpy(c_path.get(), const_c_path);
        char* token = strtok(c_path.get(), "/");
        char* last_token;
        while(token) {
            last_token = token;
            token = strtok(NULL, "/");
        }
        return last_token;
    }

}
