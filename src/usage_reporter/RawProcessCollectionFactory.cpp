#include <memory>

#include "RawProcessCollection.hpp"

#ifdef WIN32
#include "WindowsRawProcessCollection.hpp"
#endif

using std::shared_ptr;

shared_ptr<RawProcessCollection> get_raw_process_collection()
{
#ifdef WIN32
    return shared_ptr<RawProcessCollection>(new WindowsRawProcessCollection());
#endif
}