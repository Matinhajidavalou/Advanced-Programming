#pragma once
#include <string>

class Invitation {
private:
    int id;
    std::string sender;
    std::string receiver;
    std::string match_type;

public:
    Invitation(int inv_id, const std::string& snd, const std::string& rcv, const std::string& type)
        : id(inv_id), sender(snd), receiver(rcv), match_type(type) {}

    int get_id() const {
        return id; 
    }
    std::string get_sender() const {
        return sender;
    }
    std::string get_receiver() const {
        return receiver; 
    }
    std::string get_match_type() const {
        return match_type; 
    }
};