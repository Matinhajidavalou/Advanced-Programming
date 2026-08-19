#ifndef COMPANY_HPP
#define COMPANY_HPP

#include <string>
#include <vector>
#include "Order.hpp"

using namespace std;

class Company
{
public:
    string name;
    int current_price;
    vector<Order> buy_queue;
    vector<Order> sell_queue;

    Company()
    {
        name = "";
        current_price = 0;
    }

    Company(string n, int p)
    {
        name = n;
        current_price = p;
    }
};

#endif