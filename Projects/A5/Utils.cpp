#include "Utils.hpp"
#include <sstream>
#include <iostream>
#include <fstream>
#include "Exceptions.hpp"
using namespace std;
using namespace parser;

namespace parser{
map<string ,string> parse_arguments(const string& args_string){
    map<string , string> arguments;
    stringstream ss(args_string);
    string key , value;

    while(ss >> key){

        char qoute;
        ss >> qoute;
        if (qoute == '"'){
            getline(ss , value , '"');
        }
        arguments[key] = value;
    }
    return arguments;
}


pair <string , string> spliter_by_question(const string input){
    pair<string , string> splited;
    char delimiter = '?';
    size_t question_mark_pos = input.find(delimiter);

    if (question_mark_pos != string::npos) {
        splited.first = input.substr(0,question_mark_pos);
        splited.second = input.substr(question_mark_pos + 1);
    }
    else {
        throw BadRequestException();
    }

    return splited;
}

pair<string , string> extract_method(const string command ){
    pair <string , string> method_and_command;
    stringstream ss(command);
    ss >> method_and_command.first >> method_and_command.second;
    return method_and_command;
}

vector<vector<string>> read_csv(const string& path) {
    vector<vector<string>> data;
    ifstream file(path);

    if (!file.is_open()) {
        return data;
    }

    string line;
    getline(file, line);

    while (getline(file, line)) {
        if (line.empty() || line == "\r") {
             continue;
        }

        vector<string> row;
        stringstream ss(line);
        string cell;

        while (getline(ss, cell, ',')) {
            if (!cell.empty() && cell.back() == '\r') {
                cell.pop_back();
            }
            row.push_back(cell);
        }
        
        data.push_back(row);
    }
    file.close();
    return data;
}
}
