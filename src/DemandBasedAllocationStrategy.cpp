#include "../include/DemandBasedAllocationStrategy.h"
#include <algorithm>
#include <iostream>

std::vector<Allocation> DemandBasedAllocationStrategy::allocate(StockBatch& batch, std::vector<std::shared_ptr<Store>>& stores, const Date& currentDate, int& nextAllocationId) {
    std::vector<Allocation> result;
    int daysLeft = batch.getDaysToExpiry(currentDate);
    if (daysLeft <= 0) return result; // Expired stock

    std::string catName = batch.getCategory()->getName();

    struct StoreCap {
        std::shared_ptr<Store> store;
        int capacity;
    };
    std::vector<StoreCap> capacities;

    for (auto& store : stores) {
        double demand = store->getDemandRate(catName);
        if (demand > 0) {
            int cap = static_cast<int>(demand * daysLeft);
            if (cap > 0) {
                capacities.push_back({store, cap});
            }
        }
    }

    // Prioritize stores by who can sell the most (capacity desc)
    std::sort(capacities.begin(), capacities.end(), [](const StoreCap& a, const StoreCap& b) {
        return a.capacity > b.capacity;
    });

    for (auto& sc : capacities) {
        if (batch.getRemainingQuantity() == 0) break;
        
        int allocateQty = std::min(batch.getRemainingQuantity(), sc.capacity);
        if (allocateQty > 0) {
            Allocation alloc{nextAllocationId++, batch.getBatchId(), sc.store->getStoreId(), allocateQty};
            result.push_back(alloc);
            batch.reduceQuantity(allocateQty);
            
            // Add precisely this allocated quantity to the store's inventory
            StockBatch storeBatch(batch.getBatchId(), batch.getCategory(), allocateQty, batch.getArrivalDate(), batch.getExpiryDate());
            sc.store->addBatch(storeBatch);
        }
    }

    if (batch.getRemainingQuantity() > 0) {
        std::cerr << "Warning: Batch " << batch.getBatchId() << " (" << catName 
                  << ") has an unallocatable remainder of " << batch.getRemainingQuantity() << std::endl;
    }

    return result;
}

