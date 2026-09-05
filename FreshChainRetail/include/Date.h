#pragma once
#include <string>
#include <sstream>
#include <iomanip>

class Date {
private:
    int year, month, day;
public:
    Date(int y, int m, int d) : year(y), month(m), day(d) {}
    Date(const std::string& yyyymmdd) {
        char sep;
        std::stringstream ss(yyyymmdd);
        ss >> year >> sep >> month >> sep >> day;
    }
    
    int toDays() const {
        // Simplified Julian Day calculation for easy date math
        int y = year, m = month, d = day;
        if (m < 3) { y--; m += 12; }
        return 365*y + y/4 - y/100 + y/400 + (153*m - 457)/5 + d - 306;
    }
    
    int daysUntil(const Date& other) const {
        return other.toDays() - this->toDays();
    }
    
    bool operator<(const Date& other) const { return toDays() < other.toDays(); }
    bool operator<=(const Date& other) const { return toDays() <= other.toDays(); }
    bool operator==(const Date& other) const { return toDays() == other.toDays(); }
    
    std::string toString() const {
        std::stringstream ss;
        ss << year << "-" << std::setfill('0') << std::setw(2) << month << "-" << std::setw(2) << day;
        return ss.str();
    }
};

