#ifndef STOCK_H
#define STOCK_H

#include <string>
#include "../user-auth/auth.h" // when I compile: ./stock.h:5:10: fatal error: 'user-auth/auth.h' file not found
#include <sstream>
#include "json.hpp" // Include nlohmann/json
using json = nlohmann::json;

class Stock {
private:
    // const std::string& stockName;
    bool initializeDatabase() const;
    bool writeStockToDB(const std::string& stock) const;
    bool removeStockFromDB(const std::string& stock) const;
    bool updateStockQuantity(const std::string& stock) const;
    std::string convertStockFormat(const std::string& input) const;
    bool writeBookingToDB(const Auth& user, const std::string& stockName, int quantity, double bookPrice) const;
public:
    Stock();
    bool buyStock(const Auth& user, const std::string& stockName, const int bookQuantity) const;
    bool sellStock(const std::string& stockName, const int sellQuantity) const;
    void fetchRealTimeInfo(const std::string& stockName) const;
    void viewPortfolioPerformance(const std::string& stockName) const;
    json getStock(const std::string& stock) const;
    void displayPortfolio(const Auth& user) const;
};


#endif