#ifndef _PLAYER_H_
#define _PLAYER_H_

class Player {
    public:
        int stack;
        int hand[2];
        int bet;
        bool folded;

        Player();
        virtual ~Player(){}

        void buyBackIn();
        int option(int toCall);
};

#endif
