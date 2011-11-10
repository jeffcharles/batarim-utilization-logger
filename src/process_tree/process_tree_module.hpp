#ifndef GUARD_process_tree_module_h
#define GUARD_process_tree_module_h

#include <memory>

#include "ProcessTree.hpp"

std::unique_ptr<ProcessTree> get_process_tree();

#endif