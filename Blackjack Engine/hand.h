#pragma once
#include "deck.h"
#include <iostream>
#include <cmath>

class Hand {
public:
	static constexpr int NUM_INITAL_CARDS = 2; // Initial number of cards for a hand
	static constexpr int MAX_NUM_CARDS = 11; // 4 Aces, 4 Twos and 3 Threes = 21 
	static constexpr int MAX_DECK_HAND = 17; // Dealer must hit on 16 or less

	// Contructors and Destructors
	virtual ~Hand() {};
    Hand() { hand_.reserve(MAX_NUM_CARDS); };

	// Deals two cards to the hand
	void dealHand(Deck& deck);
	// Returns the value of the hand
	int getHandValue();
	void clearHand();
	// Adds a card to the hand
	void hitHand(Deck& deck);
	// Whether the hand has an Ace valued as 11
	bool isSoftHand() const { return is_hand_soft_; };

protected:
	std::vector<Card> hand_;
	bool is_hand_soft_ = false; // Whether the hand has an Ace valued as 11
};

class Dealer : public Hand {
public:
	// Possible actions for the dealer
	enum class Action { HIT, STAND };

	// Auto plays the dealer's hand
	Dealer::Action decideAction();
	void displayHand(bool hidden);
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
	Player::Action decideAction(const Card& upcard);
	// Displays the player's action to the console
	void displayAction(Player::Action action);
	// Updates the player's bankroll based on the outcome
	void updateBankroll(Player::Outcome outcome);
	void displayHand();
	// Getters
	int getBankroll() const { return bankroll_; };
	int getBet() const { return bet_; };
	bool isPlayerBroke() const { return bankroll_ == 0; };

private:
	int bankroll_; // Player's available money
	int bet_;

	// Strategies for the player
	Player::Action normalHandStrategy(const Card& upcard);
	Player::Action softHandStrategy(const Card& upcard);
	Player::Action pairHandStrategy(const Card& upcard);
};