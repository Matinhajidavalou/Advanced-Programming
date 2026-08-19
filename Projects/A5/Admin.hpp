#pragma once
#include "User.hpp"

class Admin : public User {
public:

    Admin(std::string u, std::string p) : User(u, p) {}

    bool is_admin() const override {
        return true;
    }
};