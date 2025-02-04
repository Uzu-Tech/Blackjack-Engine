#pragma once
#include "deck.h"
#include <iostream>
#include <cmath>

class Hand {
public:
	static constexpr int NUM_INITAL_CARDS = 2; // Initial number of cards for a hand
	static constexpr int MAX_NUM_CARDS = 11; // 4 Aces, 4 Twos and 3 Threes = 21 
	static constexpr int MAX_DECK_HAND = 17; // Dealer must hit on 16 or less
	static constexpr int BLACKJACK = 21; // Blackjack value

	// Contructors and Destructors
	virtual ~Hand() {};
    Hand() { hand_.reserve(MAX_NUM_CARDS); };

	// Deals two cards to the hand
	void dealHand(Deck& deck);
	// Adds a card to the hand
	void hitHand(Deck& deck);
	// Returns the value of the hand
	int getHandValue() const;
	void clearHand();
	// Whether the hand has an Ace valued as 11
	bool isSoftHand() const;
	bool isPairHand() const;

protected:
	std::vector<Card> hand_;
	int num_aces_ = 0;
};

class Dealer : public Hand {
public:
	// Possible actions for the dealer
	enum class Action { HIT, STAND };

	// Auto plays the dealer's hand
	Dealer::Action decideAction();
	void displayHand(bool hidden = true);
};

class Player : public Hand {
public:
	// Possible actions for the player
	enum class Action { HIT, STAND, DOUBLE, SPLIT };
	// Possible outcomes for the player
	enum class Outcome { BLACKJACK, WIN, LOSE };

	// Player starts with zero bet
	Player(int bankroll) : bankroll_(bankroll), bet_(0) {};

	// Calculates the bet based on the player's strategy
	void calculateBet(int min_bet, int max_bet);
	// Implements a strategy to decide the player's action
	Action decideAction(const Card& upcard) const;
	// Displays the player's action to the console
	void displayAction(Action action) const;
	// Updates the player's bankroll based on the outcome
	void updateBankroll(Outcome outcome);
	void displayHand() const;
	void displayBetAndBankroll() const;
	// Getters
	double getBankroll() const { return bankroll_; };
	double getBet() const { return bet_; };
	bool isPlayerBroke() const { return bankroll_ == 0; };

private:
	double bankroll_; // Player's available money
	double bet_;

	// Constants for the possible hand combinations
	static constexpr int NUM_HARD_HANDS = 16; // 5-20 -> 16 cards
	static constexpr int NUM_SOFT_HANDS = 8; // 13-20 -> 8 cards
	static constexpr int NUM_PAIR_HANDS = 10; // 2-A -> 10 cards
	static constexpr int NUM_DEALER_UPCARDS = 10; // 2-A -> 10 cards

	// Strategy lookup tables
	Action hard_hand_lookup[NUM_HARD_HANDS][NUM_DEALER_UPCARDS];
	Action soft_hand_lookup[NUM_SOFT_HANDS][NUM_DEALER_UPCARDS];
	Action pair_hand_lookup[NUM_PAIR_HANDS][NUM_DEALER_UPCARDS];

	// Private strategy methods for the player
	Action generateHardHandLookupTable() const;
	Action generateSoftHandLookupTable() const;
	Action generatePairHandLookupTable() const;
};