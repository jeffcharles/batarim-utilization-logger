#include <memory>

#include "IActiveWindow.hpp"
#include "WindowsActiveWindow.hpp"

#include "active_window_module.hpp"

using std::unique_ptr;

unique_ptr<IActiveWindow> get_active_window()
{
    return unique_ptr<IActiveWindow>(new WindowsActiveWindow());
}