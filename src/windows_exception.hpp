#ifndef GUARD_windows_exception_h
#define GUARD_windows_exception_h

#include <exception>
#include <sstream>
#include <string>
#include <Windows.h>

class windows_exception: public std::exception
{
    public:
        windows_exception(
            const string& m="An error occurred.",
            HRESULT hr=0
        ) : msg(m), errorcode(hr) {}
        
        virtual const char* what() const throw()
        {
            std::stringstream ret;
            ret << msg;
            if(errorcode > 0) {
                if(msg.length() > 0 && msg[msg.length()-1] != '.') {
                    ret << ".";
                }
                ret << " Error code = 0x" << hex << errorcode;
            }
            return ret.str().c_str();
        }

    protected:
        std::string msg;
        HRESULT errorcode;
};

#endif
