#pragma once
#include <vector>
#include <memory>
#include "StockBatch.h"
#include "Store.h"
#include "Allocation.h"

class AllocationStrategy {
public:
    virtual ~AllocationStrategy() = default;
    virtual std::vector<Allocation> allocate(StockBatch& batch, std::vector<std::shared_ptr<Store>>& stores, const Date& currentDate, int& nextAllocationId) = 0;
};

