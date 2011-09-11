#include <memory>

#include "FormattedProcessCollectionFactory.hpp"
#include "FormattedProcessCollection.hpp"

#ifdef WIN32
#include "WindowsFormattedProcessCollection.hpp"
#endif

#ifdef LINUX
#include "LinuxFormattedProcessCollection.hpp"
#endif

using std::shared_ptr;

shared_ptr<FormattedProcessCollection> get_formatted_process_collection()
{
#ifdef WIN32
    return shared_ptr<FormattedProcessCollection>(
        new WindowsFormattedProcessCollection());
#endif

#ifdef LINUX
    return shared_ptr<FormattedProcessCollection>(
        new LinuxFormattedProcessCollection());
#endif
}
