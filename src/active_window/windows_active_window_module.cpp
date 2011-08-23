#include <memory>

#include "IActiveWindow.hpp"
#include "WindowsActiveWindow.hpp"

#include "active_window_module.hpp"

using std::shared_ptr;

shared_ptr<IActiveWindow> get_active_window()
{
    return shared_ptr<IActiveWindow>(new WindowsActiveWindow());
}