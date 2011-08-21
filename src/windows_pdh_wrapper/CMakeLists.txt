add_library (pdh_wrapper STATIC
    pdh_wrapper.hpp PdhException.hpp pdh_wrapper.cpp PdhException.cpp)
target_link_libraries (pdh_wrapper pdh)

