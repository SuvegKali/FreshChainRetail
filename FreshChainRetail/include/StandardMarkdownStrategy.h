#pragma once
#include "MarkdownStrategy.h"
#include <map>

class StandardMarkdownStrategy : public MarkdownStrategy {
private:
    std::map<int, double> discountRules;
public:
    StandardMarkdownStrategy(std::map<int, double> rules);
    double calculatePrice(const StockBatch& batch, const Date& currentDate) const override;
};

