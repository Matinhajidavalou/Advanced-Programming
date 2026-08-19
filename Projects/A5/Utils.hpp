#pragma once
#include <string>
#include <map>
#include <vector>


namespace parser{

    std::map<std::string , std::string> parse_arguments(const std::string& argsString);
    std::pair <std::string , std::string> spliter_by_question(const std::string input);
    std::pair <std::string , std::string> extract_method(const std::string command );
    std::vector<std::vector<std::string>> read_csv(const std::string& path);

}


