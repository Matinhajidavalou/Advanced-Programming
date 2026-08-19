#include <iostream>
#include <string>
#include "System.hpp"

using namespace std;

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        return 1;
    }

    System sys;
    sys.load_companies(argv[1]);
    sys.load_shareholders(argv[2]);

    string command;
    while (cin >> command)
    {
        sys.process_command(command);
    }

    return 0;
}