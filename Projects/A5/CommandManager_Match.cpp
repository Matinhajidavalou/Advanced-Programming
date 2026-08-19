#include "CommandManager.hpp"
#include "Exceptions.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>

using namespace std;

void CommandManager::send_invitation(map<string, string>& args) {
    if (args.find("username") == args.end() || args.find("match_type") == args.end()) {
        throw BadRequestException();
    }
    
    string m_type = args["match_type"];
    if (m_type != "casual" && m_type != "ranked") {
        throw BadRequestException();
    }
    
    if (current_user == nullptr || current_user->is_admin()) {
        throw PermissionDeniedException();
    }

    string target_username = args["username"];
    if (users.find(target_username) == users.end()) {
        throw NotFoundException();
    }
    
    if (users[target_username]->is_admin()) {
        throw PermissionDeniedException();
    }

    Player* target_player = dynamic_cast<Player*>(users[target_username]);
    if (target_player->has_blocked(current_user->get_username())) {
        throw NotFoundException();
    }

    int current_inv_id = next_invitation_id;
    invitations[current_inv_id] = new Invitation(current_inv_id, current_user->get_username(), target_username, m_type);

    if (target_player != nullptr) {
        target_player->add_invitation(current_inv_id);
    }

    next_invitation_id++;
    cout << "OK" << endl;
}

void CommandManager::get_received_invitations(map<string, string>& args) {
    (void)args;
    if (current_user == nullptr || current_user->is_admin()) {
        throw PermissionDeniedException();
    }

    Player* player = dynamic_cast<Player*>(current_user);
    vector<int> inv_ids = player->get_received_invitations();

    if (inv_ids.empty()) {
        throw EmptyException();
    }

    for (int id : inv_ids) {
        if (invitations.find(id) != invitations.end()) {
            Invitation* inv = invitations[id];
            cout << id << ": Invitation from \"" << inv->get_sender() 
                 << "\" for a \"" << inv->get_match_type() << "\" match" << endl;
        }
    }
}

void CommandManager::reject_invitation(map<string, string>& args) {
    if (args.find("invitation_id") == args.end()) {
        throw BadRequestException();
    }

    if (current_user == nullptr || current_user->is_admin()) {
        throw PermissionDeniedException();
    }

    int inv_id;
    try {
        inv_id = stoi(args["invitation_id"]);
    } catch (...) {
        throw BadRequestException();
    }

    if (invitations.find(inv_id) == invitations.end()) {
        throw NotFoundException();
    }

    Player* player = dynamic_cast<Player*>(current_user);
    vector<int> user_invs = player->get_received_invitations();
    bool is_mine = false;
    
    for (int id : user_invs) {
        if (id == inv_id) {
            is_mine = true;
            break;
        }
    }

    if (!is_mine) {
        throw PermissionDeniedException();
    }

    player->remove_invitation(inv_id);
    delete invitations[inv_id];
    invitations.erase(inv_id);
    cout << "OK" << endl;
}

void CommandManager::casual_match_ready(map<string, string>& args) {
    if (args.find("status") == args.end()) {
        throw BadRequestException();
    }
    
    string status_str = args["status"];
    if (status_str != "true" && status_str != "false") {
        throw BadRequestException();
    }

    if (current_user == nullptr || current_user->is_admin()) {
        throw PermissionDeniedException();
    }

    Player* player = dynamic_cast<Player*>(current_user);
    if (status_str == "true") {
        player->set_casual_ready(true);
    } else {
        player->set_casual_ready(false);
    }

    cout << "OK" << endl;
}

void CommandManager::casual_match_opponents(map<string, string>& args) {
    if (current_user == nullptr || current_user->is_admin()) {
        throw PermissionDeniedException();
    }

    string sort_order = "desc";
    if (args.find("sort_order") != args.end()) {
        sort_order = args["sort_order"];
        if (sort_order != "asc" && sort_order != "desc") {
            throw BadRequestException();
        }
    }

    vector<Player*> opponents;
    for (auto const& [username, user_ptr] : users) {
        if (!user_ptr->is_admin() && user_ptr != current_user) {
            Player* p = dynamic_cast<Player*>(user_ptr);
            if (p != nullptr && p->is_casual_ready()) {
                opponents.push_back(p);
            }
        }
    }

    if (opponents.empty()) {
        throw EmptyException();
    }

    if (sort_order == "desc") {
        sort(opponents.begin(), opponents.end(), [](Player* a, Player* b) {
            if (a->get_xp() != b->get_xp()) {
                return a->get_xp() > b->get_xp();
            }
            return a->get_username() < b->get_username(); 
        });
    } else {
        sort(opponents.begin(), opponents.end(), [](Player* a, Player* b) {
            if (a->get_xp() != b->get_xp()) {
                return a->get_xp() < b->get_xp();
            }
            return a->get_username() < b->get_username();
        });
    }

    int rank = 1;
    for (Player* p : opponents) {
        cout << rank << ". " << p->get_username() << " with " << p->get_xp() << " XP" << endl;
        rank++;
    }
}

void CommandManager::ranked_match_opponents(map<string, string>& args) {
    if (current_user == nullptr || current_user->is_admin()) {
        throw PermissionDeniedException();
    }

    string sort_order = "desc";
    if (args.find("sort_order") != args.end()) {
        sort_order = args["sort_order"];
        if (sort_order != "asc" && sort_order != "desc") {
            throw BadRequestException();
        }
    }

    Player* me = dynamic_cast<Player*>(current_user);
    vector<Player*> opponents;
    for (auto const& [username, user_ptr] : users) {
        if (!user_ptr->is_admin() && user_ptr != me) {
            Player* p = dynamic_cast<Player*>(user_ptr);
            if (p != nullptr && p->get_level() == me->get_level()) {
                opponents.push_back(p);
            }
        }
    }

    if (opponents.empty()) {
        throw EmptyException();
    }

    if (sort_order == "desc") {
        sort(opponents.begin(), opponents.end(), [](Player* a, Player* b) {
            if (a->get_rp() != b->get_rp()) {
                return a->get_rp() > b->get_rp();
            }
            return a->get_username() < b->get_username(); 
        });
    } else {
        sort(opponents.begin(), opponents.end(), [](Player* a, Player* b) {
            if (a->get_rp() != b->get_rp()) {
                return a->get_rp() < b->get_rp();
            }
            return a->get_username() < b->get_username();
        });
    }

    int rank = 1;
    for (Player* p : opponents) {
        cout << rank << ". " << p->get_username() << " with " << p->get_rp() << " RP" << endl;
        rank++;
    }
}

void CommandManager::start_match(map<string, string>& args) {
    if (args.find("invitation_id") == args.end()) {
        throw BadRequestException();
    }

    if (current_user == nullptr || current_user->is_admin()) {
        throw PermissionDeniedException();
    }

    int inv_id;
    try {
        inv_id = stoi(args["invitation_id"]);
    } catch (...) {
        throw BadRequestException();
    }

    if (invitations.find(inv_id) == invitations.end()) {
        throw NotFoundException();
    }

    Invitation* inv = invitations[inv_id];

    if (inv->get_receiver() != current_user->get_username()) {
        throw PermissionDeniedException();
    }

    Player* receiver = dynamic_cast<Player*>(current_user);
    Player* sender = dynamic_cast<Player*>(users[inv->get_sender()]);

    if (receiver->is_playing() || sender->is_playing()) {
        throw PermissionDeniedException();
    }

    if (inv->get_match_type() == "ranked") {
        if (receiver->get_level() != sender->get_level()) {
            receiver->remove_invitation(inv_id);
            delete invitations[inv_id];
            invitations.erase(inv_id);
            throw LevelMismatchException();
        }
    }

    receiver->set_playing_status(true);
    sender->set_playing_status(true);

    Match* new_match = new Match(sender, receiver, inv->get_match_type());
    active_matches[sender->get_username()] = new_match;
    active_matches[receiver->get_username()] = new_match;

    receiver->remove_invitation(inv_id);
    delete invitations[inv_id];
    invitations.erase(inv_id);
    cout << "OK" << endl;
}

void CommandManager::play_action(map<string, string>& args) {
    if (args.find("action") == args.end()) {
        throw BadRequestException();
    }
    
    string act = args["action"];
    if (act != "shoot" && act != "defend" && act != "reload") {
        throw BadRequestException();
    }

    if (current_user == nullptr || current_user->is_admin()) {
        throw PermissionDeniedException();
    }

    string uname = current_user->get_username();
    if (active_matches.find(uname) == active_matches.end()) {
        throw NotFoundException();
    }

    Match* match = active_matches[uname];
    Player* me = dynamic_cast<Player*>(current_user);

    if (match->get_action(me) != "pending") {
        throw PermissionDeniedException();
    }

    if (act == "shoot" && match->get_bullets(me) == 0) {
        throw BadRequestException();
    }

    match->set_action(me, act);
    cout << "OK" << endl;

    if (match->both_played()) {
        match->resolve_turn();

        if (match->get_is_finished()) {
            Player* w = match->get_winner();
            Player* l = match->get_loser();

            if (match->get_match_type() == "ranked") {
                int delta_rp = 0;
                string w_lvl = w->get_level();
                
                if (w_lvl == "Bronze") {
                    delta_rp = 75;
                } else if (w_lvl == "Silver") {
                    delta_rp = 100;
                } else if (w_lvl == "Golden") {
                    delta_rp = 125;
                } else if (w_lvl == "Platinum") {
                    delta_rp = 150;
                }

                int health_bonus = match->get_health(w) * 25;
                w->set_rp(w->get_rp() + delta_rp + health_bonus);
                l->set_rp(l->get_rp() - delta_rp);
            } else {
                double xp_diff = w->get_xp() - l->get_xp();
                int delta_xp = std::max(5, (int)(50.0 - (0.1 * xp_diff)));

                w->set_xp(w->get_xp() + delta_xp);
                l->set_xp(l->get_xp() - delta_xp);
            }

            w->add_win();
            l->add_loss();
            w->set_playing_status(false);
            l->set_playing_status(false);
            active_matches.erase(w->get_username());
            active_matches.erase(l->get_username());
            
            delete match;
        }
    }
}

void CommandManager::match_status(map<string, string>& args) {
    (void)args;
    if (current_user == nullptr || current_user->is_admin()) {
        throw PermissionDeniedException();
    }

    string uname = current_user->get_username();
    if (active_matches.find(uname) == active_matches.end()) {
        throw NotFoundException();
    }
    
    Match* match = active_matches[uname];
    Player* me = dynamic_cast<Player*>(current_user);
    Player* opp;
    
    if (match->is_player1(me)) {
        opp = match->get_player2();
    } else {
        opp = match->get_player1();
    }

    cout << "Turn " << match->get_turn() << endl;
    cout << "You: " << match->get_action(me) << endl;
    
    string opp_action_display = "pending";
    if (match->get_action(opp) != "pending") {
        opp_action_display = "played";
    }
    cout << "Your opponent: " << opp_action_display << endl;

    cout << "History:" << endl;
    cout << std::left << std::setw(20) << "Opponent's moves:" << "Your moves:" << endl;
    
    vector<string> my_hist = match->get_player_history(me);
    vector<string> opp_hist = match->get_opponent_history(me);
    
    for (size_t i = 0; i < my_hist.size(); i++) {
        cout << std::left << std::setw(20) << opp_hist[i] << my_hist[i] << endl;
    }
    
    cout << "Your remaining bullets: " << match->get_bullets(me) << endl;
    
    if (match->get_match_type() == "ranked") {
        cout << "Your remaining health: " << match->get_health(me) << endl;
    }
}