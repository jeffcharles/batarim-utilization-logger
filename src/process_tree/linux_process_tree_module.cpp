#include <memory>

#include "LinuxProcessTree.hpp"

#include "process_tree_module.hpp"

using std::unique_ptr;

unique_ptr<ProcessTree> get_process_tree()
{
    return unique_ptr<ProcessTree>(new LinuxProcessTree());
}
