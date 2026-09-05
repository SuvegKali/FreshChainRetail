#include "../include/Store.h"

void Store::recordSale(const std::string& categoryName, int quantity, const Date& currentDate) {
    std::vector<StockBatch*> categoryBatches;
    
    for (auto& batch : inventory) {
        if (batch.getCategory()->getName() == categoryName && batch.getRemainingQuantity() > 0 && !batch.isExpired(currentDate)) {
            categoryBatches.push_back(&batch);
        }
    }
    
    // Sort by expiry (FEFO - First-Expiry-First-Out)
    std::sort(categoryBatches.begin(), categoryBatches.end(), [](StockBatch* a, StockBatch* b) {
        return a->getExpiryDate() < b->getExpiryDate();
    });

    int remainingToSell = quantity;
    for (auto* batch : categoryBatches) {
        if (remainingToSell <= 0) break;
        int canSell = std::min(remainingToSell, batch->getRemainingQuantity());
        batch->reduceQuantity(canSell);
        remainingToSell -= canSell;
    }

    if (remainingToSell > 0) {
        std::cerr << "Error: Store '" << name << "' tried to sell " << quantity 
                  << " of '" << categoryName << "' but only had " 
                  << (quantity - remainingToSell) << " available." << std::endl;
    }
}

std::vector<StockBatch> Store::removeExpired(const Date& currentDate) {
    std::vector<StockBatch> expired;
    std::vector<StockBatch> fresh;
    
    for (auto& batch : inventory) {
        if (batch.isExpired(currentDate) && batch.getRemainingQuantity() > 0) {
            expired.push_back(batch);
        } else {
            fresh.push_back(batch);
        }
    }
    inventory = fresh; // Keep only fresh stock
    return expired;
}

