#pragma once

struct Allocation {
    int allocationId;
    int batchId;
    int storeId;
    int quantity;
    
    int getQuantity() const { return quantity; }
    int getStoreId() const { return storeId; }
    int getBatchId() const { return batchId; }
};

