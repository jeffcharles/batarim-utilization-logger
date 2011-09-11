#ifndef GUARD_RawProcessCollectionFactory_h
#define GUARD_RawProcessCollectionFactory_h

#include <memory>

#include "RawProcessCollection.hpp"

std::shared_ptr<RawProcessCollection> get_raw_process_collection();

#endif