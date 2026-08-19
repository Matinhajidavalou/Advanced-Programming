#pragma once
#include <string>
#include <map>
#include "User.hpp"
#include "Player.hpp"
#include "Admin.hpp"
#include "Match.hpp"
#include "Invitation.hpp"
#include "Report.hpp"

class CommandManager {
private:
    std::map<std::string, User*> users;
    std::map<std::string, Match*> active_matches;
    User* current_user;
    std::map<int, Invitation*> invitations;
    int next_invitation_id;
    std::map<int, Report*> reports;
    int next_report_id;

    void process_command(const std::string& method, const std::string& command, std::map<std::string, std::string>& args);
    void registering(std::map<std::string, std::string>& args);
    void login(std::map<std::string, std::string>& args);
    void logout();
    void profile(std::map<std::string, std::string>& args);
    
    void send_invitation(std::map<std::string, std::string>& args);
    void get_received_invitations(std::map<std::string, std::string>& args);
    void reject_invitation(std::map<std::string, std::string>& args);
    void casual_match_ready(std::map<std::string, std::string>& args);
    void casual_match_opponents(std::map<std::string, std::string>& args);
    void ranked_match_opponents(std::map<std::string, std::string>& args);
    void start_match(std::map<std::string, std::string>& args);
    void play_action(std::map<std::string, std::string>& args);
    void match_status(std::map<std::string, std::string>& args);
    
    void report(std::map<std::string, std::string>& args);
    void get_reports(std::map<std::string, std::string>& args);
    void block_user(std::map<std::string, std::string>& args);
    void penalty(std::map<std::string, std::string>& args);
    void dismiss_report(std::map<std::string, std::string>& args);
    
public:
    CommandManager(const std::string& players_file, const std::string& admins_file);
    ~CommandManager();

    void run();
};