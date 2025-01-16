#ifndef AUTH_H
#define AUTH_H

#include <string>

class Auth {

private:
    std::string uname, email, pwd;
    bool authenticate(const std::string& action) const;

    bool initializeDatabase() const;

    bool signupUser(const std::string& sql) const;

    bool loginUser(const std::string& sql) const;
public:
    Auth(const std::string& uname, const std::string& email, const std::string& pwd);
    Auth(const std::string& uname, const std::string& pwd);

    bool login();
    bool signup();
    void greet(const std::string& uname, const std::string& action);
};


#endif