#include "bot.h"
#include "dealer.h"

#include <iostream>
#include <numeric>

void Bot::__init__(int* pot, const size_t opponents, int** stacks, int** bets, int* board) {
    this->potPtr = pot;
    this->numOpponents = opponents;
    this->stackPtrs = stacks;
    this->betPtrs = bets;
    this->boardPtr = board;
    this->discards.reserve(DECK_SIZE);
}

void Bot::resetBot() {
    std::vector<int> setVec(this->numOpponents);
    std::iota(setVec.begin(), setVec.end(), 0);
    this->stillIn = std::set<int>(setVec.begin(), setVec.end());
    this->discards.clear();
}

void Bot::fold(int opponent) {
    this->stillIn.erase(opponent);
}

void Bot::discard(int card) {
    this->discards.push_back(card);
    sort(this->discards.begin(), this->discards.end());
}

int Bot::option(unsigned int toCall, const int phase) {
    printf("The bot called\n");
    return std::min((int)(toCall - this->bet), this->stack);
}