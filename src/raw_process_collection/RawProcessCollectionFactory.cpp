#include <memory>

#include "RawProcessCollection.hpp"

#ifdef WIN32
#include "WindowsRawProcessCollection.hpp"
#endif

#ifdef LINUX
#include "LinuxRawProcessCollection.hpp"
#endif

using std::unique_ptr;

unique_ptr<RawProcessCollection> get_raw_process_collection()
{
#ifdef WIN32
    return unique_ptr<RawProcessCollection>(new WindowsRawProcessCollection());
#endif

#ifdef LINUX
    return unique_ptr<RawProcessCollection>(new LinuxRawProcessCollection());
#endif
}
