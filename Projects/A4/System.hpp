#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <string>
#include <map>
#include "User.hpp"
#include "Company.hpp"

using namespace std;

class System
{
private:
    map<string, User> users;
    map<string, Company> companies;
    int next_order_id = 1;

    bool has_active_order(string username, string company, OrderType type);
    bool match_buy_order(int new_id, User& u, Company& c, int shares, int price);
    void queue_buy_order(int new_id, User& u, Company& c, int shares, int price);
    bool match_sell_order(int new_id, User& u, Company& c, int shares, int price);
    void queue_sell_order(int new_id, User& u, Company& c, int shares, int price);

public:
    void load_companies(string filepath);
    void load_shareholders(string filepath);
    
    void process_command(string command);
    
    void register_user(string username, int credit);
    void buy_order(string username, int shares, string company, int price);
    void sell_order(string username, int shares, string company, int price);
    void cancel_order(int id);
    void report_portfolio(string username);
    void report_company(string company);
};

#endif