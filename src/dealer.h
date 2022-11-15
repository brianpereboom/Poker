#ifndef _DEALER_H_
#define _DEALER_H_

#include <vector>
#include <string>

#define CARDS_ON_BOARD 5
#define DECK_SIZE 52

#define PREFLOP 0
#define FLOP 3
#define TURN 4
#define RIVER 5

struct Card {
    int value;
    int suit;
    std::string label;

    Card();
    Card(int n);

    int getIndex();
};

class Dealer {
    public:
        Card cards[DECK_SIZE];
        std::vector<int> deck;

        Dealer();

        void shuffle();
        int dealCard();
};

#endif