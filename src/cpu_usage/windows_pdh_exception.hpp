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
        char* errormsg;

        void store_errormsg(const char* str)
        {
            errormsg = new char[strlen(str)+1];
            strcpy(errormsg, str);
        }
    
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

            std::string temp_string = errstream.str();
            const char* temp = temp_string.c_str();
            store_errormsg(temp);
        }

        pdh_exception(const pdh_exception& rhs)
        {
            store_errormsg(rhs.errormsg);
        }

        ~pdh_exception()
        {
            delete[] errormsg;
        }

        pdh_exception operator=(const pdh_exception rhs)
        {
            if(this != &rhs) {
                delete[] errormsg;
                store_errormsg(rhs.errormsg);
            }
            return *this;
        }

        virtual const char* what() const throw()
        {
            return errormsg;
        }
};

#endif