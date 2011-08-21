#ifndef GUARD_pdh_exception_h
#define GUARD_pdh_exception_h

#include <Pdh.h>

#include <cstring>
#include <exception>
#include <memory>
#include <sstream>
#include <string>

class PdhException: public std::exception
{
    private:
        std::shared_ptr<char> errormsg;
    
    public:
        PdhException(
            const std::string& method_name,
            const std::string& context,
            PDH_STATUS errorcode = 0
        ) {
            std::stringstream errstream;
            errstream << method_name;
            errstream << " failed while trying to ";
            errstream << context;
            errstream << ". ";
            errstream << "Error code = 0x" << std::hex << errorcode;

            const std::string temp_string = errstream.str();
            const char* temp = temp_string.c_str();
            const size_t length = strlen(temp)+1;
            errormsg = std::shared_ptr<char>(new char[length]);
            strcpy_s(errormsg.get(), length, temp);
        }

        virtual const char* what() const throw()
        {
            return errormsg.get();
        }
};

#endif