#pragma once

class StockBatch;
class Date;

class MarkdownStrategy {
public:
    virtual ~MarkdownStrategy() = default;
    virtual double calculatePrice(const StockBatch& batch, const Date& currentDate) const = 0;
};

