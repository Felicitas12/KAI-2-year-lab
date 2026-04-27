#pragma once
#include <string>


class UserAccount {
    std::string username_;
    bool loggedIn_ = false;
public:
    explicit UserAccount(std::string username) : username_(std::move(username)) {}
    bool login(const std::string& password) { return loggedIn_ = !password.empty(); }
    void logout() { loggedIn_ = false; }
    bool isLoggedIn() const { return loggedIn_; }
    const std::string& getUsername() const { return username_; }
};
