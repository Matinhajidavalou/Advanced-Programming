#pragma once
#include "User.hpp"
#include <vector>
#include <set>
#include <string>

class Player : public User {
    private:
    int xp;
    int rp;
    std::vector<int> received_invitations;
    bool casual_ready = false;
    bool in_match = false;
    int total_wins = 0;
    int total_losses = 0;
    
    std::set<std::string> blocked_users;
    int hp_penalty_amount = 0;
    int hp_penalty_matches = 0;
    int bp_penalty_amount = 0;
    int bp_penalty_matches = 0;

    public:
    
    Player(std::string u, std::string p) : User(u, p) {
        xp = 500;
        rp = 1200;
    }

    bool is_admin() const override {
        return false;
    }

    int get_xp() const {
        return xp;
    }

    void set_xp(int new_xp) {
        xp = new_xp;
    }

    int get_rp() const {
        return rp;
    }

    void set_rp(int new_rp) {
        rp = new_rp;
    }

    std::string get_level() const {
        if (rp < 1400) {
            return "Bronze";
        }
        if (rp < 1750) {
            return "Silver";
        }
        if (rp < 2250) {
            return "Golden";
        }
        return "Platinum";
    }

    void add_invitation(int inv_id) {
        received_invitations.push_back(inv_id);
    }

    std::vector<int> get_received_invitations() const {
        return received_invitations;
    }

    void remove_invitation(int inv_id) {
        for (auto it = received_invitations.begin(); it != received_invitations.end(); ++it) {
            if (*it == inv_id) {
                received_invitations.erase(it);
                break;
            }
        }
    }

    void set_casual_ready(bool status) {
        casual_ready = status;
    }

    bool is_casual_ready() const {
        return casual_ready;
    }

    void set_playing_status(bool status) {
        in_match = status;
    }

    bool is_playing() const {
        return in_match;
    }

    void add_win() {
        total_wins++;
    }

    void add_loss() {
        total_losses++;
    }

    int get_total_wins() const {
        return total_wins;
    }
    
    int get_total_losses() const {
        return total_losses;
    }

    void block(const std::string& u) {
        blocked_users.insert(u);
    }

    void unblock(const std::string& u) {
        blocked_users.erase(u);
    }

    bool has_blocked(const std::string& u) const {
        return blocked_users.find(u) != blocked_users.end();
    }

    void set_health_penalty(int amt, int matches) {
        hp_penalty_amount = amt;
        hp_penalty_matches = matches;
    }

    void set_bullet_penalty(int amt, int matches) {
        bp_penalty_amount = amt;
        bp_penalty_matches = matches;
    }
    
    int apply_health_penalty() {
        if (hp_penalty_matches > 0) {
            hp_penalty_matches--;
            return hp_penalty_amount;
        }
        return 0;
    }

    int apply_bullet_penalty() {
        if (bp_penalty_matches > 0) {
            bp_penalty_matches--;
            return bp_penalty_amount;
        }
        return 0;
    }
};