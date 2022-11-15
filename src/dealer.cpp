#include "dealer.h"

#include <numeric>
#include <random>

#define KMAG "\x1B[35m"
#define KNRM "\x1B[0m"

Card::Card() {}

Card::Card(int n) {
    this->value = n / 4;
    this->suit = n % 4;
    this->label = KMAG;
    switch (this->value) {
        case 9:
            this->label += "J";
            break;
        case 10:
            this->label += "Q";
            break;
        case 11:
            this->label += "K";
            break;
        case 12:
            this->label += "A";
            break;
        default:
            this->label += std::to_string(value + 2);
    }
    switch (this->suit)
    {
        case 0:
            this->label += "♣";
            break;
        case 1:
            this->label += "♦";
            break;
        case 2:
            this->label += "♥";
            break;
        default:
            this->label += "♠";
    }
    this->label += KNRM;
}

int Card::getIndex() {
    return this->value << 2 + this->suit;
}

Dealer::Dealer() {
    for (int i = 0; i < DECK_SIZE; i++) {
        this->cards[i] = Card(i);
    }
    this->shuffle();
}

void Dealer::shuffle() {
    this->deck = std::vector<int>(DECK_SIZE);
    std::iota(this->deck.begin(), this->deck.end(), 0);
    std::random_device rd;
    std::default_random_engine rng(rd());
    std::shuffle(this->deck.begin(), this->deck.end(), rng);
}

int Dealer::dealCard() {
    int card = this->deck.back();
    this->deck.pop_back();
    return card;
}

