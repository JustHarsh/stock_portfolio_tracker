#include "auth.h"
#include <iostream>
#include <sqlite3.h>

Auth::Auth(const std::string& uname, const std::string& email, const std::string& pwd)
    : uname(uname), email(email), pwd(pwd) {}

Auth::Auth(const std::string& uname, const std::string& pwd)
    : uname(uname), pwd(pwd) {}

bool Auth::initializeDatabase() const {
    sqlite3* db;
    sqlite3_stmt* stmt;
    char* errMsg;

    if (sqlite3_open("user.db", &db) != SQLITE_OK) {
        std::cerr << "\nError 500. Unable to open database. " << sqlite3_errmsg(db) << std::endl << std::endl << std::endl << std::endl;
        return false;
    }

    const char* createTableSQL = "CREATE TABLE IF NOT EXISTS USER ("
                                 "UNAME TEXT PRIMARY KEY, "
                                 "EMAIL TEXT NOT NULL, "
                                 "PWD TEXT NOT NULL);";

    if (sqlite3_exec(db, createTableSQL, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "\nError creating table: " << errMsg << std::endl << std::endl << std::endl << std::endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return false;
    }

    return true;
}

bool Auth::signupUser(const std::string& sql) const {
    sqlite3* db;
    sqlite3_stmt* stmt;

    if (sqlite3_open("user.db", &db) != SQLITE_OK) {
        std::cerr << "\nError 500. Unable to open database. " << sqlite3_errmsg(db) << std::endl << std::endl;
        return false;
    }

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "\nError preparing statement: " << sqlite3_errmsg(db) << std::endl << std::endl;
        sqlite3_close(db);
        return false;
    }

    // Bind parameters
    sqlite3_bind_text(stmt, 1, uname.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, email.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, pwd.c_str(), -1, SQLITE_TRANSIENT);
    

    // Execute statement
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        if (sqlite3_extended_errcode(db) == SQLITE_CONSTRAINT_PRIMARYKEY) {
            std::cerr << "\nError: Username already exists." << std::endl << std::endl;
        } else {
            std::cerr << "\nError executing statement: " << sqlite3_errmsg(db) << std::endl << std::endl;
        }
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return true;
}

bool Auth::loginUser(const std::string& sql) const {
    sqlite3* db;
    sqlite3_stmt* stmt;

    if (sqlite3_open("user.db", &db) != SQLITE_OK) {
        std::cerr << "\nError 500. Unable to open database. " << sqlite3_errmsg(db) << std::endl << std::endl;
        return false;
    }

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "\nError preparing statement: " << sqlite3_errmsg(db) << std::endl << std::endl;
        sqlite3_close(db);
        return false;
    }

    // Bind parameters
    sqlite3_bind_text(stmt, 1, uname.c_str(), -1, SQLITE_TRANSIENT);

    // Execute the query
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* storedPwd = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));

        if (pwd == storedPwd) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return true; // Authentication successful
        } else {
            std::cerr << "\nError: Incorrect password." << std::endl << std::endl;
        }
    } else {
        std::cerr << "\nError: Username does not exist." << std::endl << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return false;
}

bool Auth::authenticate(const std::string& action) const {

    if (!initializeDatabase()) {
        return false;
    }

    if (action == "SIGN_UP") {
        const std::string insertSQL = "INSERT INTO USER (UNAME, EMAIL, PWD) VALUES (?, ?, ?);";
        return signupUser(insertSQL);
    } if (action == "LOGIN") {
        const std::string fetchUserRecordSQL = "SELECT PWD FROM USER WHERE UNAME = ?;";
        return loginUser(fetchUserRecordSQL);
    }
}

void Auth::greet(const std::string& uname, const std::string& action) {
    if (action == "SIGN_UP") {
        std::cout << "\nWelcome, " << uname << "!" << std::endl << std::endl;
    } else if (action == "LOGIN") {
        std::cout << "\nWelcome back, " << uname << "!" << std::endl << std::endl;
    }
}

bool Auth::signup() {
    return authenticate("SIGN_UP");
}

bool Auth::login() {
    return authenticate("LOGIN");
}