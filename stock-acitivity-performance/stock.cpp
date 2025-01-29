#include <iostream>
#include <curl/curl.h>
#include <sqlite3.h>
#include <map>
#include <fstream>
#include <cstdlib>
#include <string>
#include <stock-acitivity-performance/stock.h>

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

std::string getStock(std::string& stock) {
    CURL* curl = curl_easy_init();

    if (curl) {
        std::string response;
        const std::string apiKey = getApiKey();
        std::string api = "https://www.alphavantage.co/query?function=GLOBAL_QUOTE&symbol=" + 
                        stock + "&apikey=" + apiKey;

        curl_easy_setopt(curl, CURLOPT_URL, api);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            std::cerr << "cURL Error: " << curl_easy_strerror(res) << "\n";
        } else {
            std::cout << "Response: " << response << "\n";
            return response;
        }

        curl_easy_cleanup(curl);

        return "GET request failed.";
    }
}

Stock::Stock(const std::string& stockName) : stockName(stockName) {}

bool Stock::initializeDatabase() const {
    sqlite3* db;
    sqlite3_stmt* stmt;
    char* errMsg;

    if (sqlite3_open("user.db", &db) != SQLITE_OK) {
        std::cerr << "\nError 500. Unable to open database. " << sqlite3_errmsg(db) << std::endl << std::endl << std::endl << std::endl;
        return false;
    }

    const char* createTableSQL = "CREATE TABLE IF NOT EXISTS PORTFOLIO ("
                                 "UNAME TEXT NOT NULL, "
                                 "STOCK TEXT NOT NULL, "
                                 "QUANTITY INT NOT NULL, "
                                 "BOOK_PRICE NUMERIC NOT NULL), "
                                 "PRIMARY KEY (UNAME, STOCK), "
                                 "FOREIGN KEY (UNAME) REFERENCES USER(UNAME) ON DELETE CASCADE, ";

    if (sqlite3_exec(db, createTableSQL, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "\nError creating table: " << errMsg << std::endl << std::endl << std::endl << std::endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return false;
    }

    return true;
}

std::string getStock(const std::string& stock) {}

bool Stock::buyStock(const std::string& stockName, const int bookQuantity) const {
    if (!initializeDatabase()) {
        return false;
    }

    std::string stockData = getStock(stockName);

    // TODO: write uname, stock, quantity, book_price to db.
}
