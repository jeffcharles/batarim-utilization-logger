#include <sstream>
#include <string>

#include "PdhException.hpp"

using std::string;
using std::stringstream;

PdhException::PdhException(
    const string& method_name,
    const string& context,
    PDH_STATUS errorcode = 0
) {
    stringstream errstream;
    errstream << method_name;
    errstream << " failed while trying to ";
    errstream << context;
    errstream << ". ";
    errstream << "Error code = 0x" << std::hex << errorcode;

    const string temp_string = errstream.str();
    const char* temp = temp_string.c_str();
    const size_t length = strlen(temp)+1;
    errormsg = std::shared_ptr<char>(new char[length]);
    strcpy_s(errormsg.get(), length, temp);
}