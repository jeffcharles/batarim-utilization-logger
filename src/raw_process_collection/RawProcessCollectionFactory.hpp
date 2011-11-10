#ifndef GUARD_RawProcessCollectionFactory_h
#define GUARD_RawProcessCollectionFactory_h

#include <memory>

#include "RawProcessCollection.hpp"

std::unique_ptr<RawProcessCollection> get_raw_process_collection();

#endif