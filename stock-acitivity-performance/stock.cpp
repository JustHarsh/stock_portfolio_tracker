#include <iostream>
#include <curl/curl.h>
#include <sqlite3.h>
#include <map>
#include <fstream>
#include <cstdlib>
#include <string>
#include "stock.h"
#include "../user-auth/auth.h"
#include <sstream>
#include "json.hpp" // Include nlohmann/json
#include <iomanip> // For formatting (setw, setprecision)

using json = nlohmann::json;

std::map<std::string, std::string> loadEnvFile(const std::string& filename) {
    std::map<std::string, std::string> envVars;
    std::ifstream file(filename);

    if (!file) {
        std::cerr << "Error: Could not open: " << filename << "\n";
        return envVars;
    }

    std::string line;
    
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);

            envVars[key] = value;

            setenv(key.c_str(), value.c_str(), 1);
        }
    }

    file.close();
    return envVars;
}

std::string getApiKey() {
    std::map<std::string, std::string> envVars = loadEnvFile(".env");
    return getenv("ALPHA_VANTAGE_API_KEY");
}


size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

json Stock::getStock(const std::string& stock) const {
    CURL* curl = curl_easy_init();
    if (curl) {
        std::string response;
        const std::string apiKey = getApiKey();
        const std::string api = "https://www.alphavantage.co/query?function=GLOBAL_QUOTE&symbol=" 
                                + stock + "&apikey=" + apiKey;
        
        curl_easy_setopt(curl, CURLOPT_URL, api.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            std::cerr << "cURL Error: " << curl_easy_strerror(res) << "\n";
            return {{"error", "GET request failed."}};
        }
        
        try {
            json j = json::parse(response);
            return j["Global Quote"];
        } catch (const json::parse_error& e) {
            std::cerr << "JSON parse error: " << e.what() << "\n";
            return {{"error", "JSON parse error"}};
        }
    }
    return {{"error", "curl_easy_init failed."}};
}


Stock::Stock() {}

bool Stock::initializeDatabase() const {
    sqlite3* db;
    sqlite3_stmt* stmt;
    char* errMsg;

    if (sqlite3_open("stock.db", &db) != SQLITE_OK) {
        std::cerr << "\nError 500. Unable to open database. " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    const char* createTableSQL = "CREATE TABLE IF NOT EXISTS PORTFOLIO ("
                                 "UNAME TEXT NOT NULL, "
                                 "STOCK TEXT NOT NULL, "
                                 "QUANTITY INT NOT NULL, "
                                 "BOOK_PRICE NUMERIC NOT NULL, "
                                 "PRIMARY KEY (UNAME, STOCK), "
                                 "FOREIGN KEY (UNAME) REFERENCES USER(UNAME) ON DELETE CASCADE) ";

    if (sqlite3_exec(db, createTableSQL, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "\nError creating table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return false;
    }
    sqlite3_close(db);
    return true;
}

// std::string getStock(const std::string& stock) {}

json parseStockData(const std::string& input) {
    json j = json::parse(input);
    return j["Global Quote"];
}

bool Stock::writeBookingToDB(const Auth& user, const std::string& stockName, int quantity, double bookPrice) const {
    sqlite3* db;
    sqlite3_stmt* stmt;

    const std::string sql = "INSERT INTO PORTFOLIO (UNAME, STOCK, QUANTITY, BOOK_PRICE) VALUES (?, ?, ?, ?);";

    if (sqlite3_open("stock.db", &db) != SQLITE_OK) {
        std::cerr << "\nError 500. Unable to open database. " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "\nError preparing statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    // Bind parameters
    sqlite3_bind_text(stmt, 1, user.getUname().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, stockName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, quantity);
    sqlite3_bind_double(stmt, 4, bookPrice);

    // Execute statement
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        if (sqlite3_extended_errcode(db) == SQLITE_CONSTRAINT_PRIMARYKEY) {
            std::cerr << "\nError: Username already exists." << std::endl;
        } else {
            std::cerr << "\nError executing statement: " << sqlite3_errmsg(db) << std::endl;
        }
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return true;
    
}

void Stock::displayPortfolio(const Auth& user) const {
    sqlite3* db;
    sqlite3_stmt* stmt;

    // SQL query to fetch portfolio data for the user
    const std::string sql = "SELECT STOCK, QUANTITY, BOOK_PRICE FROM PORTFOLIO WHERE UNAME = ?;";

    // Open database
    if (sqlite3_open("stock.db", &db) != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << "\n";
        sqlite3_close(db);
        return;
    }

    // Prepare statement
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << "\n";
        sqlite3_close(db);
        return;
    }

    // Bind username
    sqlite3_bind_text(stmt, 1, user.getUname().c_str(), -1, SQLITE_TRANSIENT);

    // Display header
    std::cout << "========================================================\n";
    std::cout << "Portfolio:\n";
    std::cout << std::left // Left-align columns
              << std::setw(12) << "Stock" 
              << std::setw(20) << "No. of shares" 
              << "Booking Price\n";

    // Fetch and display rows
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* stock = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        int quantity = sqlite3_column_int(stmt, 1);
        double bookPrice = sqlite3_column_double(stmt, 2);

        std::cout << std::setw(12) << stock 
                  << std::setw(20) << quantity 
                  << std::fixed << std::setprecision(2) << bookPrice << "\n";
    }

    // Footer
    std::cout << "========================================================\n";

    // Clean up
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

bool Stock::buyStock(const Auth& user, const std::string& stockName, const int bookQuantity) const {
    if (!initializeDatabase()) {
        return false;
    }

    json stockInfo = getStock(stockName);
    if (stockInfo.contains("error")) {
        return false;
    }
    
    std::string decisionToBuy;
    std::cout << "Are you sure you want to buy this stock? (Y or n) ";
    std::getline(std::cin, decisionToBuy);

    if (decisionToBuy == "n") {
        std::cout << "Okay, order cancelled." << std::endl;
        return false;
    }
    
    if (decisionToBuy == "Y") {
        // Display stock details
        std::cout << "Symbol: " << stockInfo["01. symbol"].get<std::string>() << std::endl
                << "Open: " << stockInfo["02. open"].get<std::string>() << std::endl
                << "High: " << stockInfo["03. high"].get<std::string>() << std::endl
                << "Low: " << stockInfo["04. low"].get<std::string>() << std::endl
                << "Price: " << stockInfo["05. price"].get<std::string>() << std::endl
                << "Volume: " << stockInfo["06. volume"].get<std::string>() << std::endl
                << "Latest Trading Day: " << stockInfo["07. latest trading day"].get<std::string>() << std::endl
                << "Previous Close: " << stockInfo["08. previous close"].get<std::string>() << std::endl
                << "Change: " << stockInfo["09. change"].get<std::string>() << std::endl
                << "Change Percent: " << stockInfo["10. change percent"].get<std::string>() << std::endl;

        double price = std::stod(stockInfo["05. price"].get<std::string>());
        double bookPrice = price * bookQuantity;

        if (writeBookingToDB(user, stockName, bookQuantity, bookPrice)) {
            std::cout << "Success! Bought " << bookQuantity << " shares of " 
                    << stockName << " at booking price: " << bookPrice << std::endl;
            //displayPortfolio(user);
            return true;
        }

        std::cout << "Something went wrong. Try again later." << std::endl;
        return false;
    }
    
    std::cout << "Bad input." << std::endl;
    return false;
}



int main() {
    Auth user("testuser", "password");
    Stock stock;

    // Insert some test data
    stock.buyStock(user, "AAPL", 10);  // Assume bookPrice = 150.31
    stock.displayPortfolio(user);

    stock.buyStock(user, "GOOGL", 12); // Assume bookPrice = 81.9942
    stock.displayPortfolio(user);
    return 0;
} // for testing/debugging purposes only. **TO BE REMOVED IN THE FINAL VERSION OF THE CODE.**
