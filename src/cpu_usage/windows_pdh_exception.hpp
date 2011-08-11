#ifndef GUARD_pdh_exception_h
#define GUARD_pdh_exception_h

#include <Pdh.h>
#include <cstring>
#include <exception>
#include <sstream>
#include <string>

class pdh_exception: public std::exception
{
    private:
        std::shared_ptr<char> errormsg;
    
    public:
        pdh_exception(
            const std::string& msg = "A PDH error occurred.",
            PDH_STATUS errorcode = 0
        )
        {
            std::stringstream errstream;
            errstream << msg;
            if(errorcode != 0) {
                if(msg.length() > 0 && msg[msg.length()-1] != '.') {
                    errstream << '.';
                }
                errstream << " Error code = 0x" << std::hex << errorcode;
            }

            const std::string temp_string = errstream.str();
            const char* temp = temp_string.c_str();
            const size_t length = strlen(temp)+1;
            char* nonconst_temp = new char[length];
            strcpy_s(nonconst_temp, length, temp);
            errormsg = std::shared_ptr<char>(nonconst_temp);
        }

        virtual const char* what() const throw()
        {
            return errormsg.get();
        }
};

#endif