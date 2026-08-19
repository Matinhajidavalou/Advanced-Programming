#include "CommandManager.hpp"
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        return 1;
    }

    try {
        CommandManager app(argv[1], argv[2]);
        app.run();
    } 
    catch (exception& e) {
        cerr << e.what() << endl;
    }

    return 0;
}