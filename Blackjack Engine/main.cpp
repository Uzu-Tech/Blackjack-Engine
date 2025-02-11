#include <iostream>
#include "deck.h"
#include "hand.h"
#include "player.h"
#include "dealer.h"
#include "game.h"
#include "rules.h"
//#include <chrono>
//using namespace std::chrono;
//auto start = high_resolution_clock::now();
//auto end = high_resolution_clock::now();
//auto duration = duration_cast<microseconds>(end - start);

// TODO - Simulate games and move strategy tables, also disable display

int main()
{
	Deck deck;
	deck.shuffleDeck();

	Player player(Settings::BANKROLL);
	Dealer dealer;

	std::cout << "Simulating Game...\n\n";
	for (long long i = 0; i < Settings::NUM_SIMULATIONS; i++) {
		playBlackjackGame(deck, player, dealer);
		deck.buildDeck();
		deck.shuffleDeck();
	}

	player.displayStats();
}