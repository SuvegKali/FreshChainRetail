#pragma once
#include <string>
#include <memory>
#include "MarkdownStrategy.h"
#include "Date.h"

class ProductCategory {
private:
    std::string name;
    int shelfLifeDays;
    double basePrice;
    std::shared_ptr<MarkdownStrategy> markdownStrategy;
public:
    ProductCategory(std::string n, int shelfLife, double price, std::shared_ptr<MarkdownStrategy> strategy)
        : name(n), shelfLifeDays(shelfLife), basePrice(price), markdownStrategy(strategy) {}
    
    std::string getName() const { return name; }
    int getShelfLifeDays() const { return shelfLifeDays; }
    double getBasePrice() const { return basePrice; }
    std::shared_ptr<MarkdownStrategy> getMarkdownStrategy() const { return markdownStrategy; }
    
    double calculatePrice(const Date& currentDate, const Date& expiryDate) const {
        return basePrice; 
    }
};

