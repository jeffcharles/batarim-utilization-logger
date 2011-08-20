add_library (pdh_wrapper STATIC
    pdh_wrapper.hpp pdh_exception.hpp pdh_wrapper.cpp)
target_link_libraries (pdh_wrapper pdh)

