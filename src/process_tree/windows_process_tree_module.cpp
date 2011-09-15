#include <memory>

#include "ProcessTree.hpp"
#include "WindowsProcessTree.hpp"

using std::shared_ptr;

shared_ptr<ProcessTree> get_process_tree()
{
    return shared_ptr<ProcessTree>(new WindowsProcessTree());
}