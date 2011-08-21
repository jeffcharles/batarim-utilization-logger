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
    public:
        PdhException(
            const std::string& method_name,
            const std::string& context,
            PDH_STATUS errorcode
        );

        virtual const char* what() const throw()
        {
            return errormsg.get();
        }
    
    private:
        std::shared_ptr<char> errormsg;
};

#endif