if (WIN32)
    add_library (process_tree STATIC
        ProcessTree.hpp WindowsProcessTree.hpp process_tree_module.hpp
        ProcessTree.cpp WindowsProcessTree.cpp windows_process_tree_module.cpp
    )
elseif (LINUX)
    add_library (process_tree STATIC
        ProcessTree.hpp LinuxProcessTree.hpp process_tree_module.hpp
        ProcessTree.cpp linux_process_tree_module.cpp
    )
endif ()

target_link_libraries (process_tree formatted_process_collection)
