#include "CommandManager.hpp"
#include "Exceptions.hpp"
#include <iostream>

using namespace std;

void CommandManager::report(map<string, string>& args) {
    if (args.find("username") == args.end() || args.find("reason") == args.end()) {
        throw BadRequestException();
    }
    
    if (args["reason"].empty()) {
        throw BadRequestException();
    }

    if (current_user == nullptr || current_user->is_admin()) {
        throw PermissionDeniedException();
    }

    string target_username = args["username"];
    if (users.find(target_username) == users.end()) {
        throw NotFoundException();
    }

    int r_id = next_report_id;
    next_report_id++;
    reports[r_id] = new Report(r_id, current_user->get_username(), target_username, args["reason"]);
    cout << "OK" << endl;
}

void CommandManager::get_reports(map<string, string>& args) {
    (void)args;
    if (current_user == nullptr || !current_user->is_admin()) {
        throw PermissionDeniedException();
    }

    if (reports.empty()) {
        throw EmptyException();
    }

    for (auto const& [r_id, rep] : reports) {
        cout << r_id << ": \"" << rep->get_sender() << "\" reported \"" 
             << rep->get_reported() << "\" for: \"" << rep->get_reason() << "\"" << endl;
    }
}

void CommandManager::penalty(map<string, string>& args) {
    if (current_user == nullptr || !current_user->is_admin()) {
        throw PermissionDeniedException();
    }
    
    if (args.find("report_id") == args.end() || args.find("type") == args.end() ||
        args.find("amount") == args.end() || args.find("number_of_matches") == args.end()) {
        throw BadRequestException();
    }

    int r_id;
    int amt;
    int matches;
    
    try {
        r_id = stoi(args["report_id"]);
        amt = stoi(args["amount"]);
        matches = stoi(args["number_of_matches"]);
    } catch (...) {
        throw BadRequestException();
    }

    string type = args["type"];
    
    if (type != "health_penalty" && type != "bullet_penalty") {
        throw BadRequestException();
    }
    
    if (type == "health_penalty" && (amt < 1 || amt > 2)) {
        throw BadRequestException();
    }
    
    if (type == "bullet_penalty" && (amt < 1 || amt > 3)) {
        throw BadRequestException();
    }
    
    if (matches < 1) {
        throw BadRequestException();
    }

    if (reports.find(r_id) == reports.end()) {
        throw NotFoundException();
    }

    Report* rep = reports[r_id];
    Player* reported_player = dynamic_cast<Player*>(users[rep->get_reported()]);
    
    if (reported_player != nullptr) {
        if (type == "health_penalty") {
            reported_player->set_health_penalty(amt, matches);
        } else {
            reported_player->set_bullet_penalty(amt, matches);
        }
    }

    delete reports[r_id];
    reports.erase(r_id);
    cout << "OK" << endl;
}

void CommandManager::dismiss_report(map<string, string>& args) {
    if (current_user == nullptr || !current_user->is_admin()) {
        throw PermissionDeniedException();
    }
    
    if (args.find("report_id") == args.end()) {
        throw BadRequestException();
    }

    int r_id;
    try {
        r_id = stoi(args["report_id"]);
    } catch (...) {
        throw BadRequestException();
    }

    if (reports.find(r_id) == reports.end()) {
        throw NotFoundException();
    }

    delete reports[r_id];
    reports.erase(r_id);
    cout << "OK" << endl;
}