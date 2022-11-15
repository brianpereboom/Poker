#ifndef _TABLE_H_
#define _TABLE_H_

#include "dealer.h"
#include "bot.h"
#include "player.h"

class Table : public Dealer, public Bot {
    public:
        int ante;
        int smallBlind;
        int bigBlind;
        std::vector<Player*> opponents;
        int button;
        int* board;
        int pot;

        Table();
        ~Table() {
            for (int i = 0; i < this->opponents.size(); i++)
                delete this->opponents[i];
            free(board);
        }

        void moveButton();
        void antes();
        void blinds();
        void deal();
        void flop();
        void turn();
        void river();
        bool bettingCycle(int phase);
        bool collect();
        void rejoin();
        void showDown();
};

#endif