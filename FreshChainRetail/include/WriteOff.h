#pragma once
#include <string>
#include "Date.h"
#include "ProductCategory.h"

struct WriteOff {
    int batchId;
    std::string categoryName;
    int quantity;
    double value;
    Date writeOffDate;
};

