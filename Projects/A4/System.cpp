#include "System.hpp"
#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>

void System::load_companies(string filepath)
{
    ifstream file(filepath);
    string line;
    bool is_header = true;
    while (getline(file, line))
    {
        if (is_header)
        {
            is_header = false;
            continue;
        }
        vector<string> parts = split(line, ',');
        if (parts.size() >= 2)
        {
            string name = parts[0];
            int price = stoi(parts[1]);
            companies[name] = Company(name, price);
        }
    }
}

void System::load_shareholders(string filepath)
{
    ifstream file(filepath);
    string line;
    bool is_header = true;
    while (getline(file, line))
    {
        if (is_header)
        {
            is_header = false;
            continue;
        }
        vector<string> parts = split(line, ',');
        if (parts.size() >= 3)
        {
            string username = parts[0];
            int credit = stoi(parts[1]);
            User u(username, credit);
            
            vector<string> shares_list = split(parts[2], ';');
            for (size_t i = 0; i < shares_list.size(); i++)
            {
                vector<string> share_info = split(shares_list[i], ':');
                if (share_info.size() == 2)
                {
                    u.free_shares[share_info[0]] = stoi(share_info[1]);
                }
            }
            users[username] = u;
        }
    }
}

void System::process_command(string command)
{
    if (command == "register")
    {
        string username;
        int credit;
        cin >> username >> credit;
        register_user(username, credit);
    }
    else if (command == "buy_order")
    {
        string username, company;
        int shares, price;
        cin >> username >> shares >> company >> price;
        buy_order(username, shares, company, price);
    }
    else if (command == "sell_order")
    {
        string username, company;
        int shares, price;
        cin >> username >> shares >> company >> price;
        sell_order(username, shares, company, price);
    }
    else if (command == "cancel_order")
    {
        int id;
        cin >> id;
        cancel_order(id);
    }
    else if (command == "report_portfolio")
    {
        string username;
        cin >> username;
        report_portfolio(username);
    }
    else if (command == "report_company")
    {
        string company;
        cin >> company;
        report_company(company);
    }
}

void System::register_user(string username, int credit)
{
    if (users.find(username) != users.end())
    {
        cout << username << " already exists." << endl;
    }
    else
    {
        users[username] = User(username, credit);
        cout << username << " registered successfully." << endl;
    }
}

bool System::has_active_order(string username, string company, OrderType type)
{
    Company& c = companies[company];
    const vector<Order>& queue = (type == BUY) ? c.buy_queue : c.sell_queue;
    for (size_t i = 0; i < queue.size(); i++)
    {
        if (queue[i].username == username)
        {
            return true;
        }
    }
    return false;
}

bool System::match_buy_order(int new_id, User& u, Company& c, int shares, int price)
{
    for (size_t i = 0; i < c.sell_queue.size(); i++)
    {
        if (c.sell_queue[i].shares == shares && c.sell_queue[i].price == price)
        {
            int matched_id = c.sell_queue[i].id;
            User& seller = users[c.sell_queue[i].username];

            u.free_credit -= (shares * price);
            seller.locked_shares[c.name] -= shares;
            seller.free_credit += (shares * price);
            u.free_shares[c.name] += shares;

            c.current_price = price;
            c.sell_queue.erase(c.sell_queue.begin() + i);

            cout << "Order " << new_id << " matched with order " << matched_id << "." << endl;
            return true;
        }
    }
    return false;
}

void System::queue_buy_order(int new_id, User& u, Company& c, int shares, int price)
{
    u.free_credit -= (shares * price);
    u.locked_credit += (shares * price);
    
    Order o;
    o.id = new_id;
    o.type = BUY;
    o.username = u.username;
    o.company = c.name;
    o.shares = shares;
    o.price = price;
    
    c.buy_queue.push_back(o);
    cout << "Order " << new_id << " queued." << endl;
}

void System::buy_order(string username, int shares, string company_name, int price)
{
    User& u = users[username];
    if (u.free_credit < shares * price)
    {
        cout << "Insufficient free credit." << endl;
        return;
    }
    if (has_active_order(username, company_name, SELL))
    {
        cout << username << " already has a sell order queued for " << company_name << "." << endl;
        return;
    }

    int new_id = next_order_id++;
    Company& c = companies[company_name];

    if (!match_buy_order(new_id, u, c, shares, price))
    {
        queue_buy_order(new_id, u, c, shares, price);
    }
}

bool System::match_sell_order(int new_id, User& u, Company& c, int shares, int price)
{
    for (size_t i = 0; i < c.buy_queue.size(); i++)
    {
        if (c.buy_queue[i].shares == shares && c.buy_queue[i].price == price)
        {
            int matched_id = c.buy_queue[i].id;
            User& buyer = users[c.buy_queue[i].username];

            u.free_shares[c.name] -= shares;
            buyer.locked_credit -= (shares * price);
            u.free_credit += (shares * price);
            buyer.free_shares[c.name] += shares;

            c.current_price = price;
            c.buy_queue.erase(c.buy_queue.begin() + i);

            cout << "Order " << new_id << " matched with order " << matched_id << "." << endl;
            return true;
        }
    }
    return false;
}

void System::queue_sell_order(int new_id, User& u, Company& c, int shares, int price)
{
    u.free_shares[c.name] -= shares;
    u.locked_shares[c.name] += shares;
    
    Order o;
    o.id = new_id;
    o.type = SELL;
    o.username = u.username;
    o.company = c.name;
    o.shares = shares;
    o.price = price;
    
    c.sell_queue.push_back(o);
    cout << "Order " << new_id << " queued." << endl;
}

void System::sell_order(string username, int shares, string company_name, int price)
{
    User& u = users[username];
    if (u.free_shares[company_name] < shares)
    {
        cout << "Insufficient free shares." << endl;
        return;
    }
    if (has_active_order(username, company_name, BUY))
    {
        cout << username << " already has a buy order queued for " << company_name << "." << endl;
        return;
    }

    int new_id = next_order_id++;
    Company& c = companies[company_name];

    if (!match_sell_order(new_id, u, c, shares, price))
    {
        queue_sell_order(new_id, u, c, shares, price);
    }
}

void System::cancel_order(int id)
{
    for (auto& pair : companies)
    {
        Company& c = pair.second;
        
        for (size_t i = 0; i < c.buy_queue.size(); i++)
        {
            if (c.buy_queue[i].id == id)
            {
                User& u = users[c.buy_queue[i].username];
                u.locked_credit -= (c.buy_queue[i].shares * c.buy_queue[i].price);
                u.free_credit += (c.buy_queue[i].shares * c.buy_queue[i].price);
                c.buy_queue.erase(c.buy_queue.begin() + i);
                cout << "Canceled order " << id << "." << endl;
                return;
            }
        }
        
        for (size_t i = 0; i < c.sell_queue.size(); i++)
        {
            if (c.sell_queue[i].id == id)
            {
                User& u = users[c.sell_queue[i].username];
                u.locked_shares[c.name] -= c.sell_queue[i].shares;
                u.free_shares[c.name] += c.sell_queue[i].shares;
                c.sell_queue.erase(c.sell_queue.begin() + i);
                cout << "Canceled order " << id << "." << endl;
                return;
            }
        }
    }
    cout << "Order " << id << " not found." << endl;
}

void System::report_portfolio(string username)
{
    if (users.find(username) == users.end())
    {
        cout << username << " not found." << endl;
        return;
    }
    
    User& u = users[username];
    long long total_free_assets = u.free_credit;
    for (const auto& pair : u.free_shares)
    {
        total_free_assets += (pair.second * companies[pair.first].current_price);
    }

    cout << username << " Portfolio" << endl;
    cout << "Total free assets: $" << total_free_assets << endl;
    cout << "Free credit: $" << u.free_credit << endl;
    cout << "Locked credit: $" << u.locked_credit << endl;
    
    cout << "Free shares:" << endl;
    bool has_free = false;
    int idx = 1;
    for (const auto& pair : u.free_shares)
    {
        if (pair.second > 0)
        {
            cout << idx++ << ". " << pair.first << ": " << pair.second << endl;
            has_free = true;
        }
    }
    if (!has_free)
    {
        cout << "(empty)" << endl;
    }

    cout << "Locked shares:" << endl;
    bool has_locked = false;
    idx = 1;
    for (const auto& pair : u.locked_shares)
    {
        if (pair.second > 0)
        {
            cout << idx++ << ". " << pair.first << ": " << pair.second << endl;
            has_locked = true;
        }
    }
    if (!has_locked)
    {
        cout << "(empty)" << endl;
    }
}

void System::report_company(string company_name)
{
    if (companies.find(company_name) == companies.end())
    {
        cout << company_name << " not found." << endl;
        return;
    }

    Company& c = companies[company_name];
    cout << c.name << " report" << endl;
    cout << "Current price: $" << c.current_price << endl;

    vector<Order> sorted_sell = c.sell_queue;
    sort(sorted_sell.begin(), sorted_sell.end(), [](const Order& a, const Order& b)
    {
        if (a.price != b.price)
        {
            return a.price < b.price;
        }
        return a.id < b.id;
    });

    cout << "sell queue:" << endl;
    if (sorted_sell.empty())
    {
        cout << "(empty)" << endl;
    }
    else
    {
        for (size_t i = 0; i < sorted_sell.size(); i++)
        {
            cout << (i + 1) << ". Shares: " << sorted_sell[i].shares << "\n   Price: $" << sorted_sell[i].price << "\n   ID: " << sorted_sell[i].id << endl;
        }
    }

    vector<Order> sorted_buy = c.buy_queue;
    sort(sorted_buy.begin(), sorted_buy.end(), [](const Order& a, const Order& b)
    {
        if (a.price != b.price)
        {
            return a.price > b.price;
        }
        return a.id < b.id;
    });

    cout << "buy queue:" << endl;
    if (sorted_buy.empty())
    {
        cout << "(empty)" << endl;
    }
    else
    {
        for (size_t i = 0; i < sorted_buy.size(); i++)
        {
            cout << (i + 1) << ". Shares: " << sorted_buy[i].shares << "\n   Price: $" << sorted_buy[i].price << "\n   ID: " << sorted_buy[i].id << endl;
        }
    }
}