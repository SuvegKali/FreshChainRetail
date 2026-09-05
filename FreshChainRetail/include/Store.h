#pragma once
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <algorithm>
#include "StockBatch.h"
#include "ProductCategory.h"

class Store {
private:
    int storeId;
    std::string name;
    std::map<std::string, double> demandRates; // Category Name -> demand rate
    std::vector<StockBatch> inventory;
public:
    Store(int id, std::string n) : storeId(id), name(n) {}

    int getStoreId() const { return storeId; }
    std::string getName() const { return name; }
    
    void setDemandRate(const std::string& categoryName, double rate) {
        demandRates[categoryName] = rate;
    }
    
    double getDemandRate(const std::string& categoryName) const {
        auto it = demandRates.find(categoryName);
        return it != demandRates.end() ? it->second : 0.0;
    }

    void addBatch(const StockBatch& batch) {
        inventory.push_back(batch);
    }

    const std::vector<StockBatch>& getInventory() const { return inventory; }

    void recordSale(const std::string& categoryName, int quantity, const Date& currentDate);
    std::vector<StockBatch> removeExpired(const Date& currentDate);
};

