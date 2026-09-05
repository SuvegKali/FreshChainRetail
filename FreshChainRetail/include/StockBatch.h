#pragma once
#include "ProductCategory.h"
#include "Date.h"
#include <memory>

class StockBatch {
private:
    int batchId;
    std::shared_ptr<ProductCategory> category;
    int initialQuantity;
    int remainingQuantity;
    Date arrivalDate;
    Date expiryDate;
    double unitPrice;
public:
    StockBatch(int id, std::shared_ptr<ProductCategory> cat, int quantity, Date arrival, Date expiry)
        : batchId(id), category(cat), initialQuantity(quantity), remainingQuantity(quantity),
          arrivalDate(arrival), expiryDate(expiry), unitPrice(cat->getBasePrice()) {}

    int getBatchId() const { return batchId; }
    std::shared_ptr<ProductCategory> getCategory() const { return category; }
    int getRemainingQuantity() const { return remainingQuantity; }
    Date getArrivalDate() const { return arrivalDate; }
    Date getExpiryDate() const { return expiryDate; }
    double getUnitPrice() const { return unitPrice; }
    
    int getDaysToExpiry(const Date& currentDate) const {
        return currentDate.daysUntil(expiryDate);
    }
    
    void reduceQuantity(int quantity) {
        remainingQuantity -= quantity;
    }
    
    bool isExpired(const Date& currentDate) const {
        return expiryDate < currentDate;
    }
    
    double getCurrentPrice(const Date& currentDate) const {
        if (category && category->getMarkdownStrategy()) {
            return category->getMarkdownStrategy()->calculatePrice(*this, currentDate);
        }
        return unitPrice;
    }
};

