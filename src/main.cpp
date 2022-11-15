#include "table.h"
#include "player.h"
#include "dealer.h"

#include <iostream>

// Waits for user to initiate the next hand
void newHand() {
    printf("Play again? Press ENTER to continue!");
    getchar();
}

int main() {
    Table* table = new Table();
    while (true) {
        table->antes();
        printf("Pot(ante): %i\n", table->pot);
        table->blinds();
        printf("Pot(blinds): %i\n", table->pot);

        table->deal();

        printf("Bot:\t\t\t%s\t%s\n", table->cards[table->hand[0]].label.c_str(), table->cards[table->hand[1]].label.c_str());
        for (int i = 0; i < table->opponents.size(); i++)
            printf("Player %i:\t\t%s\t%s\n", i, table->cards[table->opponents[i]->hand[0]].label.c_str(), table->cards[table->opponents[i]->hand[1]].label.c_str());
        if (table->bettingCycle(PREFLOP)) {
            table->flop();
            table->turn();
            table->river();
            goto showdown;
        }
        printf("Pot(opening): %i\n", table->pot);
        
        table->flop();
        if (table->bettingCycle(FLOP)) {
            table->turn();
            table->river();
            goto showdown;
        }
        printf("Pot(flop): %i\n", table->pot);

        table->turn();
        if (table->bettingCycle(TURN)) {
            table->river();
            goto showdown;
        }
        printf("Pot(turn): %i\n", table->pot);

        table->river();
        table->bettingCycle(RIVER);
        printf("Pot(river): %i\n", table->pot);

        showdown:
            table->showDown();

        newHand();

        table->rejoin();
        table->shuffle();
        table->moveButton();
    }
    delete table;
}