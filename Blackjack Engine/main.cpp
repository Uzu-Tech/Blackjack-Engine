#include <iostream>
#include "deck.h"
#include "hand.h"
#include "players.h"
//#include <chrono>
//using namespace std::chrono;
//auto start = high_resolution_clock::now();
//auto end = high_resolution_clock::now();
//auto duration = duration_cast<microseconds>(end - start);

int main()
{
	constexpr int BANKROLL = 10000;
    Deck deck;
    deck.shuffleDeck();

    Dealer dealer;
    Player player(BANKROLL);
    dealer.dealHand(deck);

	player.calculateBet(1, 10);
	player.dealHand(deck);

    player.displayBetAndBankroll();
	player.displayHand();
	bool hidden = true;
    dealer.displayHand();
	hidden = false;

    if (dealer.decideAction() == Dealer::Action::HIT) {
        dealer.hitHand(deck);
        dealer.displayHand(hidden);
    }
}