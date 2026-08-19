#include "CommandManager.hpp"
#include "Exceptions.hpp"
#include <iostream>

using namespace std;

void CommandManager::registering(map<string, string>& args) {
    if (current_user != nullptr) {
        throw PermissionDeniedException();
    }

    if (args.find("username") == args.end() || args.find("password") == args.end()) {
        throw BadRequestException();
    }

    string user = args["username"];
    string pass = args["password"];

    if (users.find(user) != users.end()) {
        throw BadRequestException();
    }

    users[user] = new Player(user, pass);
    current_user = users[user];
    cout << "OK" << endl;
}

void CommandManager::login(map<string, string>& args) {
    if (current_user != nullptr) {
        throw PermissionDeniedException();
    }

    if (args.find("username") == args.end() || args.find("password") == args.end()) {
        throw BadRequestException();
    }

    string user = args["username"];
    string pass = args["password"];

    if (users.find(user) == users.end()) {
        throw NotFoundException();
    }

    if (!users[user]->check_password(pass)) {
        throw PermissionDeniedException();
    }

    current_user = users[user];
    cout << "OK" << endl;
}

void CommandManager::logout() {
    if (current_user == nullptr) {
        throw PermissionDeniedException();
    }
    
    current_user = nullptr;
    cout << "OK" << endl;
}

void CommandManager::profile(map<string, string>& args) {
    if (current_user == nullptr) {
        throw PermissionDeniedException();
    }

    string target_username;

    if (args.find("username") == args.end()) {
        if (current_user->is_admin()) {
            throw PermissionDeniedException();
        }
        target_username = current_user->get_username();
    } else {
        target_username = args["username"];
    }

    if (users.find(target_username) == users.end()) {
        throw NotFoundException();
    }

    User* target_user = users[target_username];

    if (target_user->is_admin()) {
        throw PermissionDeniedException();
    }

    Player* player_ptr = dynamic_cast<Player*>(target_user);
    if (player_ptr != nullptr) {
        cout << "username: \"" << player_ptr->get_username() << "\"" << endl;
        cout << "Level: " << player_ptr->get_level() << endl;
        cout << "RP: " << player_ptr->get_rp() << endl;
        cout << "XP: " << player_ptr->get_xp() << endl;
        cout << "Total wins: " << player_ptr->get_total_wins() << endl;
        cout << "Total losses: " << player_ptr->get_total_losses() << endl;
    }
}

void CommandManager::block_user(map<string, string>& args) {
    if (args.find("username") == args.end() || args.find("status") == args.end()) {
        throw BadRequestException();
    }
    
    string target = args["username"];
    string status = args["status"];
    
    if (status != "blocked" && status != "unblocked") {
        throw BadRequestException();
    }
    
    if (current_user == nullptr || current_user->is_admin()) {
        throw PermissionDeniedException();
    }
    
    if (users.find(target) == users.end()) {
        throw NotFoundException();
    }
    
    if (users[target]->is_admin()) {
        throw BadRequestException();
    }

    Player* me = dynamic_cast<Player*>(current_user);
    if (status == "blocked") {
        me->block(target);
    } else {
        me->unblock(target);
    }

    cout << "OK" << endl;
}