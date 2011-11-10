#include <memory>

#include "ProcessTree.hpp"
#include "WindowsProcessTree.hpp"

using std::unique_ptr;

unique_ptr<ProcessTree> get_process_tree()
{
    return unique_ptr<ProcessTree>(new WindowsProcessTree());
}