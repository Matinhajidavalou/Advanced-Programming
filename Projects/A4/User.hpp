#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <map>

using namespace std;

class User
{
public:
    string username;
    int free_credit;
    int locked_credit;
    map<string, int> free_shares;
    map<string, int> locked_shares;

    User()
    {
        username = "";
        free_credit = 0;
        locked_credit = 0;
    }

    User(string u, int c)
    {
        username = u;
        free_credit = c;
        locked_credit = 0;
    }
};

#endif