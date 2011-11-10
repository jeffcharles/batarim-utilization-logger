#ifndef GUARD_FormattedProcessCollectionFactory_h
#define GUARD_FormattedProcessCollectionFactory_h

#include <memory>

#include "FormattedProcessCollection.hpp"

std::unique_ptr<FormattedProcessCollection> get_formatted_process_collection();

#endif