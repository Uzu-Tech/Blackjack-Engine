#include <iostream>
#include "deck.h"
#include "hand.h"
//#include <chrono>
//using namespace std::chrono;
//auto start = high_resolution_clock::now();
//auto end = high_resolution_clock::now();
//auto duration = duration_cast<microseconds>(end - start);

int main()
{
    Deck deck;
    deck.shuffleDeck();

    Dealer dealer;
    dealer.dealHand(deck);
    dealer.displayHand();

    if (dealer.decideAction() == Dealer::Action::HIT) {
        dealer.hitHand(deck);
        dealer.displayHand();
    }
}