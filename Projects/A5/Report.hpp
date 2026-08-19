#pragma once
#include <string>

class Report {
private:
    int id;
    std::string sender;
    std::string reported;
    std::string reason;

public:
    Report(int i, const std::string& s, const std::string& r, const std::string& msg) 
        : id(i), sender(s), reported(r), reason(msg) {}
    
    int get_id() const { return id; }
    std::string get_sender() const {
        return sender;
    }
    std::string get_reported() const {
        return reported;
    }
    std::string get_reason() const {
        return reason;
    }
};