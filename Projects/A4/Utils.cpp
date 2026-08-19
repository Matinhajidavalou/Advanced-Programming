#include "Utils.hpp"
#include <sstream>

string trim(const string& str)
{
    size_t first = str.find_first_not_of(" \t\r\n");
    if (string::npos == first)
    {
        return "";
    }
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

vector<string> split(const string& s, char delimiter)
{
    vector<string> tokens;
    string token;
    stringstream token_stream(s);
    while (getline(token_stream, token, delimiter))
    {
        string trimmed = trim(token);
        if (!trimmed.empty())
        {
            tokens.push_back(trimmed);
        }
    }
    return tokens;
}