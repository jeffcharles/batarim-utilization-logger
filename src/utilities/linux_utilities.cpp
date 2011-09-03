#include <cstring>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>

#include "linux_utilities.hpp"

using std::ifstream;
using std::string;
using std::stringstream;
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

    string get_process_name(unsigned int pid)
    {
        stringstream filepath;
        filepath << "/proc/" << pid << "/cmdline";
        
        ifstream cmdline_stream;
        cmdline_stream.open(filepath.str());
        
        string command;
        cmdline_stream >> command;
        
        cmdline_stream.close();
        
        return get_filename_from_path(command);
    }

}
