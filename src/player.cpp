#include "player.h"

#include <iostream>

Player::Player() {
    std::string num;
    printf("\tSet buy in: ");
    std::getline(std::cin, num);
    this->stack = std::stoi(num);
    this->hand[0] = -1;
    this->hand[0] = -1;
    this->folded = true;
}

void Player::buyBackIn() {
    std::string num;
    printf("\tSet buy in: ");
    std::getline(std::cin, num);
    this->stack = std::stoi(num);
}

int Player::option(int toCall) {
    if (toCall == 0) {
        std::string num;
        printf("\tYour option. Enter an amount to bet (check = 0, all in = %i): ", this->stack);
        std::getline(std::cin, num);
        try {
            return std::min(std::stoi(num), this->stack);
            throw std::invalid_argument("");
        } catch (std::invalid_argument const& e) {
            return 0;
        }
    }
    if (toCall == this->bet) {
        std::string num;
        printf("\tYour option. Press ENTER to check or enter an amount to bet (check = 0, all in = %i): ", this->stack);
        std::getline(std::cin, num);
        try {
            return std::min(std::min(std::stoi(num), this->stack), 0);
            throw std::invalid_argument("");
        } catch (std::invalid_argument const& e) {
            return 0;
        }
    }
    printf("\tYour option. Press ENTER to fold or enter an amount to which to raise (call = %i, all in = %i): ", std::min(toCall - this->bet, this->stack), this->stack);
    std::string num;
    std::getline(std::cin, num);
    if (num == "") {
        this->folded = true;
        return 0;
    }
    try {
        int amt = std::stoi(num);
        if (amt < std::min(toCall - this->bet, this->stack)) {
            this->folded = true;
            return 0;
        }
        return std::min(amt, this->stack);
        throw std::invalid_argument("");
    } catch (std::invalid_argument const& e) {
        this->folded = true;
        return 0;
    }
}