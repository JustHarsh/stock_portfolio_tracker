#ifndef STOCK_H
#define STOCK_H

#include <string>

class Stock {
private:
    const std::string& stockName;
    bool initializeDatabase() const;
    bool writeStockToDB(const std::string& stock) const;
    bool removeStockFromDB(const std::string& stock) const;
    bool updateStockQuantity(const std::string& stock) const;
public:
    Stock(const std::string& stockName);
    bool buyStock(const std::string& stockName, const int bookQuantity) const;
    bool sellStock(const std::string& stockName, const int sellQuantity) const;
    void fetchRealTimeInfo(const std::string& stockName) const;
    void viewPortfolioPerformance(const std::string& stockName) const;
    std::string getStock(const std::string& stock) const;
};


#endif