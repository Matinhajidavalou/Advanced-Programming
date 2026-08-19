#pragma once
#include <string>
#include <vector>
#include "Player.hpp"

class Match {
private:
    Player* player1;
    Player* player2;
    std::string match_type;
    
    int p1_bullets;
    int p2_bullets;
    int p1_health;
    int p2_health;
    std::string p1_action;
    std::string p2_action;
    
    std::vector<std::string> p1_history;
    std::vector<std::string> p2_history;
    
    int turn_number;
    bool is_finished;
    Player* winner;
    Player* loser;

public:
    Match(Player* p1, Player* p2, std::string type) : player1(p1), player2(p2), match_type(type) {
        if (match_type == "ranked") {
            p1_bullets = 3 - p1->apply_bullet_penalty();
            if (p1_bullets < 0) {
                p1_bullets = 0;
            }
            p2_bullets = 3 - p2->apply_bullet_penalty();
            if (p2_bullets < 0) {
                p2_bullets = 0;
            }
            
            p1_health = 3 - p1->apply_health_penalty();
            if (p1_health <= 0) {
                p1_health = 1;
            }
            p2_health = 3 - p2->apply_health_penalty();
            if (p2_health <= 0) {
                p2_health = 1;
            }
        } else {
            p1_bullets = 1;
            p2_bullets = 1;
            p1_health = 1;
            p2_health = 1;
        }
        
        p1_action = "pending";
        p2_action = "pending";
        turn_number = 1;
        is_finished = false;
        winner = nullptr;
        loser = nullptr;
    }
    virtual ~Match() = default;

    std::string get_match_type() const {
        return match_type;
    }

    Player* get_player1() const {
        return player1;
    }

    Player* get_player2() const {
        return player2;
    }

    int get_turn() const {
        return turn_number;
    }

    bool get_is_finished() const {
        return is_finished;
    }

    Player* get_winner() const {
        return winner;
    }

    Player* get_loser() const {
        return loser;
    }

    bool is_player1(Player* p) const {
        return p == player1;
    }

    int get_bullets(Player* p) const {
        if (is_player1(p)) {
            return p1_bullets;
        }
        return p2_bullets;
    }

    int get_health(Player* p) const {
        if (is_player1(p)) {
            return p1_health;
        }
        return p2_health;
    }

    std::string get_action(Player* p) const {
        if (is_player1(p)) {
            return p1_action;
        }
        return p2_action;
    }
    
    void set_action(Player* p, const std::string& action) {
        if (is_player1(p)) {
            p1_action = action;
        } else {
            p2_action = action;
        }
    }

    bool both_played() const {
        return p1_action != "pending" && p2_action != "pending";
    }

    void resolve_turn() {
        p1_history.push_back(p1_action);
        p2_history.push_back(p2_action);

        if (p1_action == "reload") {
            p1_bullets++;
        }
        if (p1_action == "shoot") {
            p1_bullets--;
        }
        
        if (p2_action == "reload") {
            p2_bullets++;
        }
        if (p2_action == "shoot") {
            p2_bullets--;
        }

        if (p1_action == "shoot" && p2_action == "reload") {
            if (match_type == "ranked") {
                p2_health--;
                if (p2_health <= 0) {
                    is_finished = true;
                    winner = player1;
                    loser = player2;
                }
            } else {
                is_finished = true;
                winner = player1;
                loser = player2;
            }
        }
        else if (p2_action == "shoot" && p1_action == "reload") {
            if (match_type == "ranked") {
                p1_health--;
                if (p1_health <= 0) {
                    is_finished = true;
                    winner = player2;
                    loser = player1;
                }
            } else {
                is_finished = true;
                winner = player2;
                loser = player1;
            }
        }

        if (!is_finished) {
            turn_number++;
            p1_action = "pending";
            p2_action = "pending";
        }
    }

    std::vector<std::string> get_player_history(Player* p) const { 
        if (is_player1(p)) {
            return p1_history;
        }
        return p2_history;
    }

    std::vector<std::string> get_opponent_history(Player* p) const { 
        if (is_player1(p)) {
            return p2_history;
        }
        return p1_history;
    }
};