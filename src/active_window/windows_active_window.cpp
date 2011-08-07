#include <string>
#include <Windows.h>

#include "active_window.hpp"

using std::wstring;

wstring get_active_window_name()
{
    HWND active_window = GetForegroundWindow();
    const int MAX_WINDOW_NAME_LENGTH = 255;
    TCHAR window_name[MAX_WINDOW_NAME_LENGTH];
    int window_text_retrieved_length = GetWindowText(
        active_window, // handle to window to retrieve name for
        window_name,  // non-const TCHAR string to output to
        MAX_WINDOW_NAME_LENGTH // maximum length of text to copy
    );

    if(window_text_retrieved_length == 0) {
        return L"";
    } else {
        // wstring does not have a constructor that takes a wchar_t, therefore
        // need to copy contents from the wchar_t buffer into a wstring
        wstring ret;
        ret.assign(&window_name[0], 
            &window_name[window_text_retrieved_length]);
        return ret;
    }
}
