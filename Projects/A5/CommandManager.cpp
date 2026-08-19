#include "CommandManager.hpp"
#include "Utils.hpp"
#include "Exceptions.hpp"
#include <iostream>
#include <set>

using namespace std; 
using namespace parser;

CommandManager::CommandManager(const string& players_file, const string& admins_file) {
    current_user = nullptr;
    next_invitation_id = 1;
    next_report_id = 1;

    vector<vector<string>> players_data = read_csv(players_file);
    for (const auto& row : players_data) {
        if (row.size() >= 3) {
            Player* new_player = new Player(row[0], row[1]);
            new_player->set_xp(stoi(row[2]));
            
            if (row.size() >= 4) {
                new_player->set_rp(stoi(row[3]));
            }
            
            users[row[0]] = new_player;
        }
    }

    vector<vector<string>> admins_data = read_csv(admins_file);
    for (const auto& row : admins_data) {
        if (row.size() >= 2) {
            users[row[0]] = new Admin(row[0], row[1]);
        }
    }
}

CommandManager::~CommandManager() {
    for (auto const& [key, user_ptr] : users) {
        delete user_ptr;
    }
    users.clear();

    std::set<Match*> unique_matches;
    for (auto const& [uname, match_ptr] : active_matches) {
        unique_matches.insert(match_ptr);
    }

    for (Match* match_ptr : unique_matches) {
        delete match_ptr;
    }
    active_matches.clear();

    for (auto const& [key, inv_ptr] : invitations) {
        delete inv_ptr;
    }
    invitations.clear();

    for (auto const& [key, rep_ptr] : reports) {
        delete rep_ptr;
    }
    reports.clear();
}

void CommandManager::run() {
    string line;
    while (getline(cin, line)) {
        if (line.empty() || line == "\r") {
            continue;
        }

        try {
            pair<string, string> separated = spliter_by_question(line);
            pair<string, string> method_and_command = extract_method(separated.first);
            map<string, string> args = parse_arguments(separated.second);
            process_command(method_and_command.first, method_and_command.second, args);
        } 
        catch (const exception& e) {
            cout << e.what() << endl;
        }
    }
}

void CommandManager::process_command(const string& method, const string& command, map<string, string>& args) {
    if (method == "POST" && command == "register") {
        registering(args);
    }
    else if (method == "POST" && command == "login") {
        login(args);
    }
    else if (method == "POST" && command == "logout") {
        logout();
    }
    else if (method == "GET" && command == "profile") {
        profile(args);
    }
    else if (method == "POST" && command == "invitation") {
        send_invitation(args);
    }
    else if (method == "GET" && command == "received_invitations") {
        get_received_invitations(args);
    }
    else if (method == "POST" && command == "reject_invitation") {
        reject_invitation(args);
    }
    else if (method == "POST" && command == "casual_match_ready") {
        casual_match_ready(args);
    }
    else if (method == "GET" && command == "casual_match_opponents") {
        casual_match_opponents(args);
    }
    else if (method == "GET" && command == "ranked_match_opponents") {
        ranked_match_opponents(args);
    }
    else if (method == "POST" && command == "start_match") {
        start_match(args);
    }
    else if (method == "POST" && command == "action") {
        play_action(args);
    }
    else if (method == "GET" && command == "match_status") {
        match_status(args);
    }
    else if (method == "POST" && command == "report"){
        report(args);
    }
    else if (method == "GET" && command == "reports"){
        get_reports(args);
    }
    else if (method == "POST" && command == "block") {
        block_user(args);
    }
    else if (method == "POST" && command == "penalty") {
        penalty(args);
    }
    else if (method == "POST" && command == "dismiss_report") {
        dismiss_report(args);
    }
    else {
        throw NotFoundException();
    }
}