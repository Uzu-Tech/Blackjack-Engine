#include <iostream>
#include "deck.h"
#include "hand.h"
#include "player.h"
#include "dealer.h"
#include "game.h"
//#include <chrono>
//using namespace std::chrono;
//auto start = high_resolution_clock::now();
//auto end = high_resolution_clock::now();
//auto duration = duration_cast<microseconds>(end - start);

// TODO - Simulate games and move strategy tables, also disable display

int main()
{
	constexpr int BANKROLL = 10000;

	Deck deck;
	deck.shuffleDeck();

	Player player(BANKROLL);
	Dealer dealer;

	playBlackjackGame(deck, player, dealer);
}