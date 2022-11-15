#ifndef _BOT_H_
#define _BOT_H_

#include "player.h"

#include <set>
#include <vector>

class Bot : public Player {
    int* potPtr;
    int numOpponents;
    int** stackPtrs;
    int** betPtrs;
    int* boardPtr;
    std::set<int> stillIn;
    std::vector<int> discards;

    public:
        Bot() {}
        ~Bot() {
            delete this->potPtr;
            delete this->stackPtrs;
            delete this->betPtrs;
            delete this->boardPtr;
        }

        void __init__(int* pot, const size_t opponents, int** stacks, int** bets, int* board);
        void resetBot();

        void nextAction(unsigned char player, unsigned int bet);
        void fold(int opponent);
        void discard(int card);

        int option(unsigned int toCall, const int phase);
};

#endif