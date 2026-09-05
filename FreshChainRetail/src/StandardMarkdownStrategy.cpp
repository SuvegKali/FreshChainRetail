#include "../include/StandardMarkdownStrategy.h"
#include "../include/StockBatch.h"
#include <algorithm>

StandardMarkdownStrategy::StandardMarkdownStrategy(std::map<int, double> rules) : discountRules(rules) {}

double StandardMarkdownStrategy::calculatePrice(const StockBatch& batch, const Date& currentDate) const {
    int daysLeft = batch.getDaysToExpiry(currentDate);
    double maxDiscount = 0.0;
    
    for (auto const& [threshold, discount] : discountRules) {
        if (daysLeft <= threshold) {
            maxDiscount = std::max(maxDiscount, discount);
        }
    }
    
    return batch.getUnitPrice() * (1.0 - maxDiscount);
}

