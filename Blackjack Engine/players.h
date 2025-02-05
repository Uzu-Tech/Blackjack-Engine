#pragma once
#include "deck.h"
#include "hand.h"

class Dealer {
public:
	// Possible actions for the dealer
	enum class Action { HIT, STAND };

	// Hand methods
	void dealHand(Deck& deck) { hand_.dealHand(deck); };
	void hitHand(Deck& deck) { hand_.hitHand(deck); };

	// Auto plays the dealer's hand
	Action decideAction();
	void displayHand(bool hidden = true) const;

private:
	Hand hand_ = Hand();
};

class Player {
public:
	// Possible actions for the player
	enum class Action { HIT, STAND, DOUBLE, SPLIT };
	// Possible outcomes for the player
	enum class Outcome { BLACKJACK, WIN, LOSE };

	// Player starts with zero bet
	Player(int bankroll) : bankroll_(bankroll), bet_(0),
		hard_hand_lookup_{}, soft_hand_lookup_{}, pair_hand_lookup_{} {
		// Generate strategy lookup tables
		generateHardHandLookupTable_();
		generateSoftHandLookupTable_();
		generatePairHandLookupTable_();
	};

	// Hand methods
	void dealHand(Deck& deck) { current_hand_->dealHand(deck); };
	void hitHand(Deck& deck) { 
		if (has_doubled_) {
			throw std::runtime_error("Cannot hit after doubling down");
		}
		current_hand_->hitHand(deck); 
	};

	// Calculates the bet based on the player's strategy
	void calculateBet(int min_bet, int max_bet);
	// Implements a strategy to decide the player's action
	Action decideAction(const Card& upcard) const;
	// Actions
	void doubleDown(Deck& deck);
	void splitHand(Deck& deck);
	// Switch to the split hand
	void swapHand() {
		if (!has_split_) {
			throw std::runtime_error("Cannot swap hands without splitting");
		}

		current_hand_ = &split_hand_;
	};
	// Displays the player's action to the console
	void displayAction(Action action) const;
	// Updates the player's bankroll based on the outcome
	void updateBankroll(Outcome outcome);
	void displayHand() const;
	void displayBetAndBankroll() const;
	// Getters
	bool hasSplit() const { return has_split_; };
	bool hasDoubled() const { return has_doubled_; };
	double getBankroll() const { return bankroll_; };
	double getBet() const { return bet_; };
	bool isBroke() const { return bankroll_ == 0; };

private:
	double bankroll_; // Player's available money
	double bet_;

	bool has_split_ = false;
	bool has_doubled_ = false;
	// Hands
	Hand hand_ = Hand();
	Hand split_hand_ = Hand();
	Hand* current_hand_ = &hand_;

	// Strategy lookup tables
	Action hard_hand_lookup_[HandConstants::NUM_HARD_HANDS][HandConstants::NUM_DEALER_UPCARDS];
	Action soft_hand_lookup_[HandConstants::NUM_SOFT_HANDS][HandConstants::NUM_DEALER_UPCARDS];
	Action pair_hand_lookup_[HandConstants::NUM_PAIR_HANDS][HandConstants::NUM_DEALER_UPCARDS];

	// Private strategy methods for the player
	void generateHardHandLookupTable_();
	void generateSoftHandLookupTable_();
	void generatePairHandLookupTable_();
};