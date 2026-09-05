#include "../include/DistributionCenter.h"
#include <iostream>

void DistributionCenter::receiveBatch(StockBatch batch, const Date& currentDate) {
    if (batch.isExpired(currentDate)) {
        std::cerr << "Rejected intake: Batch " << batch.getBatchId() << " is already past its expiry date." << std::endl;
        return;
    }
    batches.push_back(batch);
}

void DistributionCenter::allocateBatch(int batchId, const Date& currentDate) {
    for (auto& batch : batches) {
        if (batch.getBatchId() == batchId) {
            auto newAllocs = allocationStrategy->allocate(batch, stores, currentDate, nextAllocationId);
            allocations.insert(allocations.end(), newAllocs.begin(), newAllocs.end());
            return;
        }
    }
    std::cerr << "Error: Batch " << batchId << " not found for allocation." << std::endl;
}

void DistributionCenter::processWriteOffs(const Date& currentDate) {
    for (auto& store : stores) {
        auto expired = store->removeExpired(currentDate);
        for (auto& b : expired) {
            double val = b.getRemainingQuantity() * b.getUnitPrice();
            writeOffs.push_back({b.getBatchId(), b.getCategory()->getName(), b.getRemainingQuantity(), val, currentDate});
            std::cout << "[Write-off] Store '" << store->getName() << "' | Batch " << b.getBatchId() 
                      << " | Qty " << b.getRemainingQuantity() << " | Value $" << val << std::endl;
        }
    }
    
    // Process unallocated DC stock
    for (auto& b : batches) {
        if (b.isExpired(currentDate) && b.getRemainingQuantity() > 0) {
            double val = b.getRemainingQuantity() * b.getUnitPrice();
            writeOffs.push_back({b.getBatchId(), b.getCategory()->getName(), b.getRemainingQuantity(), val, currentDate});
            std::cout << "[Write-off] DC Unallocated | Batch " << b.getBatchId() 
                      << " | Qty " << b.getRemainingQuantity() << " | Value $" << val << std::endl;
            b.reduceQuantity(b.getRemainingQuantity());
        }
    }
}

void DistributionCenter::queryStoreInventory(int storeId, const Date& currentDate) const {
    for (auto& store : stores) {
        if (store->getStoreId() == storeId) {
            std::cout << "--- Current Inventory for '" << store->getName() << "' ---" << std::endl;
            for (auto& b : store->getInventory()) {
                if (b.getRemainingQuantity() > 0) {
                    std::cout << "Category: " << b.getCategory()->getName() 
                              << " | Batch: " << b.getBatchId() 
                              << " | Qty: " << b.getRemainingQuantity()
                              << " | Days to Expiry: " << b.getDaysToExpiry(currentDate)
                              << " | Price: $" << b.getCurrentPrice(currentDate) 
                              << std::endl;
                }
            }
            return;
        }
    }
}

StockBatch* DistributionCenter::getBatch(int batchId) {
    for (auto& batch : batches) {
        if (batch.getBatchId() == batchId) return &batch;
    }
    return nullptr;
}

