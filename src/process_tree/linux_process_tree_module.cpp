#include <memory>

#include "LinuxProcessTree.hpp"

#include "process_tree_module.hpp"

using std::shared_ptr;

shared_ptr<ProcessTree> get_process_tree()
{
    return shared_ptr<ProcessTree>(new LinuxProcessTree());
}
