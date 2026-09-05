#pragma once
#include <vector>
#include <memory>
#include "Store.h"
#include "StockBatch.h"
#include "Allocation.h"
#include "WriteOff.h"
#include "AllocationStrategy.h"

class DistributionCenter {
private:
    std::vector<std::shared_ptr<Store>> stores;
    std::vector<StockBatch> batches;
    std::vector<Allocation> allocations;
    std::vector<WriteOff> writeOffs;
    std::shared_ptr<AllocationStrategy> allocationStrategy;
    int nextAllocationId = 1;

public:
    DistributionCenter(std::shared_ptr<AllocationStrategy> strategy) : allocationStrategy(strategy) {}

    void addStore(std::shared_ptr<Store> store) {
        stores.push_back(store);
    }

    void receiveBatch(StockBatch batch, const Date& currentDate);
    void allocateBatch(int batchId, const Date& currentDate);
    void processWriteOffs(const Date& currentDate);
    void queryStoreInventory(int storeId, const Date& currentDate) const;
    StockBatch* getBatch(int batchId);
};

