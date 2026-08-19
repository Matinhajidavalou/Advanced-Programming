#pragma once
#include <string>
#include <utility>

class User{
    protected:
    
        std::string username;
        std::string password;

    public:
    User(){
        username="";
        password="";
    }
    User(std::string u , std::string p){

        username=u;
        password=p;
    }    
    
    virtual ~User() = default;

    std::string get_username() const {
        return username;
    }

    bool check_password(const std::string& input_pass) const {
        return password == input_pass;
    }

    virtual bool is_admin() const = 0;




};