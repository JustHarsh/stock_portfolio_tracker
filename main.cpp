#include <iostream>
#include <string>
#include <limits>
#include "user-auth/auth.h"

void clearInputBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

bool registerUser() {
    std::string uname, email, pwd;

    clearInputBuffer();
    std::cout << "Enter username: ";
    std::getline(std::cin, uname);

    std::cout << "Enter email: ";
    std::getline(std::cin, email);

    std::cout << "Enter password: ";
    std::getline(std::cin, pwd);

    Auth user(uname, email, pwd);

    if (user.signup()) {
        std::cout << "Signup successful, happy trading!\n";
        user.greet(uname, "SIGN_UP");
        return true;
    }

    return false;
}

bool loginUser() {
    std::string uname, pwd;

    clearInputBuffer();
    std::cout << "Enter username: ";
    std::getline(std::cin, uname);

    std::cout << "Enter password: ";
    std::getline(std::cin, pwd);

    Auth user(uname, pwd);

    if (user.login()) {
        std::cout << "Login Successful!\n";
        user.greet(uname, "LOGIN");
        return true;
    }

    return false;
}

int main() {
    int authChoice;
    bool isAuthenticated = false;

    std::cout << "Welcome to the Portfolio Performance Tracker!\n";

    do {
        std::cout << "Select an option (type 1 or 2):\n";
        std::cout << "1. Register\n";
        std::cout << "2. Login\n";

        std::cout << "Enter your choice: ";
        std::cin >> authChoice;

        if (std::cin.fail() || (authChoice != 1 && authChoice != 2)) {
            std::cin.clear();
            clearInputBuffer();
            std::cout << "Invalid choice. Please try again.\n";
            continue;
        }

        if (authChoice == 1) {
            if (registerUser()) {
                isAuthenticated = true;
            }
        } else if (authChoice == 2) {
            if (loginUser()) {
                isAuthenticated = true;
            }
        }
    } while (!isAuthenticated);

    std::cout << "Display trading module...\n";
    std::cout << "Select an option:\n";
    std::cout << "1. Buy stock\n2. Sell stock\n"
                << "3. View Stock (fetch real-time data)\n"
                << "4. View Portfolio Performance\n5. Exit\n";
    return 0;
}
