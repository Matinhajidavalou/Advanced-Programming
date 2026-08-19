#ifndef ORDER_HPP
#define ORDER_HPP

#include <string>

using namespace std;

enum OrderType
{
    BUY,
    SELL
};

class Order
{
public:
    int id;
    OrderType type;
    string username;
    string company;
    int shares;
    int price;
};

#endif