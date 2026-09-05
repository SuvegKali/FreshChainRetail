#pragma once
#include "AllocationStrategy.h"

class DemandBasedAllocationStrategy : public AllocationStrategy {
public:
    std::vector<Allocation> allocate(StockBatch& batch, std::vector<std::shared_ptr<Store>>& stores, const Date& currentDate, int& nextAllocationId) override;
};

