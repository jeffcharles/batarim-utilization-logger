#include <memory>
#include <string>

#include <Windows.h>

using std::string;
using std::unique_ptr;
using std::wstring;

namespace batarim {
    namespace encoding_methods {
        
        string convert_wstring_to_string(wstring wide_str)
        {
            // Run WideCharToMultiByte once with NULL output buffer to get
            // required size of output buffer. Run it a second time with a
            // non-NULL output buffer to populate the output buffer.
            // See
            // http://msdn.microsoft.com/en-us/library/dd374130(v=vs.85).aspx
            // for details.
            const int buffer_size = WideCharToMultiByte(
                CP_UTF8, // encoding to convert to (i.e., UTF-16 to UTF-8)
                0, // conversion flags (none look necessary)
                wide_str.c_str(), // UTF-16 buffer to convert from
                wide_str.length()+1, // size of UTF-16 buffer
                NULL, // buffer receiving UTF-8 string
                0, // size of UTF-8 buffer (0 means calculate required size)
                NULL, // default character (must be NULL for UTF-8)
                NULL // ptr to indicate if default char used (NULL for UTF-8)
            );
            unique_ptr<char> multibyte_buffer(new char[buffer_size]);
            WideCharToMultiByte(
                CP_UTF8,
                0,
                wide_str.c_str(),
                wide_str.length()+1,
                multibyte_buffer.get(), // buffer receiving UTF-8 string
                buffer_size, // size of UTF-8 buffer
                NULL,
                NULL
            );
            return string(multibyte_buffer.get());
        }
    }
}