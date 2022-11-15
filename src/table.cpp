#include "table.h"

#include <phevaluator/phevaluator.h>

#include <iostream>
#include <random>
#include <map>
#include <cstring>

#define MAX_OPPONENTS 22
#define MAX_RANK 7462

Table::Table() {
    this->pot = 0;
    std::string num;
    printf("Set ante: ");
    std::getline(std::cin, num);
    this->ante = std::stoi(num);
    printf("Set small blind: ");
    std::getline(std::cin, num);
    this->smallBlind = std::stoi(num);
    printf("Set big blind: ");
    std::getline(std::cin, num);
    this->bigBlind = std::stoi(num);
    printf("Set number of opponents: ");
    std::getline(std::cin, num);
    int opponents = std::min(std::stoi(num), MAX_OPPONENTS);
    for (int i = 0; i < opponents; i++) {
        printf("Player %i:\n", i);
        this->opponents.push_back(new Player());
    }
    srand(time(NULL));
    this->button = rand() % (this->opponents.size() + 1);
    this->board = (int*)calloc(CARDS_ON_BOARD, sizeof(int));
    std::fill(this->board, this->board + CARDS_ON_BOARD, -1);
    int** stackPtrs = (int**)calloc(opponents, sizeof(int*));
    int** betPtrs = (int**)calloc(opponents, sizeof(int*));
    for (int i = 0; i < opponents; i++) {
        *(stackPtrs + i) = &(this->opponents[i]->stack);
        *(betPtrs + i) = &(this->opponents[i]->bet);
    }
    __init__(&(this->pot), opponents, stackPtrs, betPtrs, this->board);
    this->resetBot();
}

void Table::moveButton() {
    this->button++;
    if (this->button > this->opponents.size()) {
        this->button -= this->opponents.size() + 1;
    }
}

void Table::antes() {
    int amt = std::min(this->ante, this->stack);
    this->bet = amt;
    this->stack -= amt;
    this->pot += amt;
    for (Player* i : this->opponents) {
        amt = std::min(this->ante, i->stack);
        i->bet = amt;
        i->stack -= amt;
        this->pot += amt;
    }
    this->collect();
}

void Table::blinds() {
    int i = this->button + 1;
    if (i > this->opponents.size()) {
        i -= this->opponents.size() + 1;
    }
    if (i == this->opponents.size()) {
        int amt = std::min(this->smallBlind, this->stack);
        this->bet = amt;
        this->stack -= amt;
        this->pot += amt;
        i -= this->opponents.size();
    } else {
        int amt = std::min(this->smallBlind, this->opponents[i]->stack);
        this->opponents[i]->bet = amt;
        this->opponents[i]->stack -= amt;
        this->pot += amt;
        i++;
    }
    if (i == this->opponents.size()) {
        int amt = std::min(this->bigBlind, this->stack);
        this->bet = amt;
        this->stack -= amt;
        this->pot += amt;
    } else {
        int amt = std::min(this->bigBlind, this->opponents[i]->stack);
        this->opponents[i]->bet = amt;
        this->opponents[i]->stack -= amt;
        this->pot += amt;
    }
}

void Table::deal() {
    this->hand[0] = this->dealCard();
    this->discard(this->hand[0]);
    this->hand[1] = this->dealCard();
    this->discard(this->hand[1]);
    this->folded = false;
    for (Player* i : this->opponents) {
        i->hand[0] = this->dealCard();
        i->hand[1] = this->dealCard();
        i->folded = false;
    }
}

void Table::flop() {
    this->board[0] = this->dealCard();
    this->discard(this->board[0]);
    this->board[1] = this->dealCard();
    this->discard(this->board[1]);
    this->board[2] = this->dealCard();
    this->discard(this->board[2]);
    printf("Flop:\t\t\t%s\t%s\t%s\n", this->cards[this->board[0]].label.c_str(), this->cards[this->board[1]].label.c_str(), this->cards[this->board[2]].label.c_str());
}

void Table::turn() {
    this->board[3] = this->dealCard();
    this->discard(this->board[3]);
    printf("Turn:\t\t\t%s\n", this->cards[this->board[3]].label.c_str());
}

void Table::river() {
    this->board[4] = this->dealCard();
    this->discard(this->board[4]);
    printf("River:\t\t\t%s\n", this->cards[this->board[4]].label.c_str());
}

bool Table::bettingCycle(int phase) {
    int toCall = 0;
    int timer = this->opponents.size() + 1;
    int i = this->button + 1;
    if (phase == PREFLOP) {
        toCall = this->bigBlind;
        i += 2;
    }
    if (i > this->opponents.size())
        i %= this->opponents.size() + 1;
    while (timer > 0) {
        for (int j = 0; j < this->opponents.size() + 1; j++) {
            if (i == this->opponents.size()) {
                if (!this->folded && this->stack != 0) {
                    printf("Bot:\n");
                    int amt = this->option(toCall, phase); // Bot's bet
                    this->bet += amt;
                    if (this->bet > toCall) {
                        toCall = this->bet;
                        timer = this->opponents.size() + 1;
                    }
                    this->stack -= amt;
                    this->pot += amt;
                    if (this->folded)
                        printf("\tDiscards:\t%s\t%s\n", this->cards[this->hand[0]].label.c_str(), this->cards[this->hand[1]].label.c_str());
                }
                i -= this->opponents.size();
            } else {
                if (!this->opponents[i]->folded && this->opponents[i]->stack != 0) {
                    printf("Player %i:\n", i);
                    int amt = this->opponents[i]->option(toCall);
                    this->opponents[i]->bet += amt;
                    if (this->opponents[i]->bet > toCall) {
                        toCall = this->opponents[i]->bet;
                        timer = this->opponents.size() + 1;
                    }
                    this->opponents[i]->stack -= amt;
                    this->pot += amt;
                    if (this->opponents[i]->folded) {
                        this->fold(i);
                        std::string show;
                        printf("\tShow?(y/N): ");
                        std::getline(std::cin, show);
                        if (show == "Y" || show == "y") {
                            printf("\tDiscards:\t%s\t%s\n", this->cards[this->opponents[i]->hand[0]].label.c_str(), this->cards[this->opponents[i]->hand[1]].label.c_str());
                            this->discard(this->opponents[i]->hand[0]);
                            this->discard(this->opponents[i]->hand[1]);
                        }
                    }
                }
                i++;
            }
            timer--;
            if (timer == 0)
                break;
        }
    }
    return this->collect();
}

bool Table::collect() {
    int stillIn = 0;
    if (!this->folded && this->stack != 0)
        stillIn++;
    this->bet = 0;
    for (Player* i : this->opponents) {
        if (!i->folded && i->stack != 0)
            stillIn++;
        i->bet = 0;
    }
    return (stillIn > 1) ? false : true;
}

void Table::rejoin() {
    if (this->stack == 0) {
        printf("Bot:\n");
        this->buyBackIn();
    }
    for (int i = 0; i < this->opponents.size(); i++) {
        if (this->opponents[i]->stack == 0) {
            printf("Player %i:\n", i);
            this->opponents[i]->buyBackIn();
        }
    }
}

int aceHigh(int val) {
    return (val == 0) ? 12 : val - 1;
}

void Table::showDown() {
    int ranks[this->opponents.size() + 1];
    int best = MAX_RANK;
    if (this->folded)
        ranks[this->opponents.size()] = MAX_RANK;
    else {
        ranks[this->opponents.size()] = phevaluator::EvaluateCards(this->hand[0], this->hand[1], board[0], board[1], board[2], board[3], board[4]).value();
        best = ranks[this->opponents.size()];
    }
    for (int i = 0; i < this->opponents.size(); i++) {
        if (this->opponents[i]->folded)
            ranks[i] = MAX_RANK;
        else {
            ranks[i] = phevaluator::EvaluateCards(this->opponents[i]->hand[0], this->opponents[i]->hand[1], board[0], board[1], board[2], board[3], board[4]).value();
            best = std::min(best, ranks[i]);
        }
    }
    std::vector<int> winners;
    for (int i = 0; i < this->opponents.size() + 1; i++)
        if (ranks[i] == best)
            winners.push_back(i);
    if (winners.size() == 1)
        if (winners[0] == this->opponents.size()) {
            printf("Winner: Bot with %s\n", describe_rank(best));
            this->stack += this->pot;
        } else {
            printf("Winner: Player %i with %s\n", winners[0], describe_rank(best));
            this->opponents[winners[0]]->stack += this->pot;
        }
    else {
        printf("Split pot with %s:\n", describe_rank(best));
        int splitPot = this->pot / winners.size();
        for (int i : winners) {
            if (i == this->opponents.size()) {
                printf("\tBot\n");
                this->stack += splitPot;
            } else {
                printf("\tPlayer %i\n", i);
                this->opponents[i]->stack += splitPot;
            }
        }
        this->pot -= splitPot * winners.size();
        if (this->pot != 0)
            printf("Odd chip: %i\n", this->pot);
    }
    this->pot = 0;
    std::fill(this->board, this->board + CARDS_ON_BOARD, -1);
    this->resetBot();
}