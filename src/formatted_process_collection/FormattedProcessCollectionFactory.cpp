#include <memory>

#include "FormattedProcessCollectionFactory.hpp"
#include "FormattedProcessCollection.hpp"

#ifdef WIN32
#include "WindowsFormattedProcessCollection.hpp"
#endif

#ifdef LINUX
#include "LinuxFormattedProcessCollection.hpp"
#endif

using std::unique_ptr;

unique_ptr<FormattedProcessCollection> get_formatted_process_collection()
{
#ifdef WIN32
    return unique_ptr<FormattedProcessCollection>(
        new WindowsFormattedProcessCollection());
#endif

#ifdef LINUX
    return unique_ptr<FormattedProcessCollection>(
        new LinuxFormattedProcessCollection());
#endif
}
