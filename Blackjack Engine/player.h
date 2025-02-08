#pragma once
#include "deck.h"
#include "hand.h"
#include <unordered_map>
#include <cmath>

class Player {
public:
	// Possible actions for the player
	enum class Action { HIT, STAND, DOUBLE, SPLIT, DOUBLE_OR_STAND };
	// Possible outcomes for the player
	enum class Outcome { BLACKJACK, BUST, STAND, SPLIT };
	// Results for displaying to terminal
	enum class Result { BLACKJACK, BUST, WIN, LOSE, TIE, DEALER_BUST, DEALER_BLACKJACK };

	// Player starts with zero bet
	Player(int bankroll) : bankroll_(bankroll), bet_(0), split_bet_(0),
		hard_hand_lookup_{}, soft_hand_lookup_{}, pair_hand_lookup_{} {
		// Generate strategy lookup tables
		generateHardHandLookupTable_();
		generateSoftHandLookupTable_();
		generatePairHandLookupTable_();
	};

	// Hand methods
	void dealHand(Deck& deck) { current_hand_->dealHand(deck); };
	// Debugging method to set the player's hand
	void setHand(const Card& card1, const Card& card2) {
		current_hand_->addCard(card1);
		current_hand_->addCard(card2);
	}
	void hitHand(Deck& deck) { 
		if (has_doubled_ && !has_split_) {
			throw std::runtime_error("Cannot hit after doubling down");
		}
		current_hand_->hitHand(deck); 
	};

	// Calculates the bet based on the player's strategy
	void calculateBet(int max_bankroll);
	// Implements a strategy to decide the player's action
	Action decideAction(const Card& upcard) const;
	// Actions
	void doubleDown(Deck& deck);
	void splitHand(Deck& deck);
	// Switch hands
	void switchToSplitHand()
	{
		if (!has_split_) {
			throw std::runtime_error("Cannot switch to the split hand without splitting");
		}

		current_bet_ = &split_bet_;
		current_hand_ = &split_hand_;
	}
	void switchToNormalHand() {
		if (!has_split_) {
			throw std::runtime_error("Cannot revert to normal hand if hands aren't split");
		}

		current_hand_ = &hand_;
		current_bet_ = &bet_;
	}
	// Outcome checks
	bool isBust() const { return current_hand_->getHandValue() > HandConstants::BLACKJACK; };
	bool hasBlackjack() const { return current_hand_->getHandValue() == HandConstants::BLACKJACK; };
	// Displays the player's action to the console
	void displayAction(Action action) const;
	void displayResult(Player::Result result) const;
	// Updates the player's bankroll based on the outcome
	Player::Result updateBankroll(int dealer_hand_value);
	void displayHand() const;
	void displayBetAndBankroll() const;

	void reset();

	// Getters
	bool hasSplit() const { return has_split_; };
	bool hasDoubled() const { return has_doubled_; };
	bool isOnSplitHand() const { return current_hand_ == &split_hand_; };
	double getBankroll() const { return bankroll_; };
	double getBet() const { return bet_; };
	bool isBroke() const { return bankroll_ == 0; };
	size_t getHandLength() const { return current_hand_->getHand().size(); }


private:
	double bankroll_; // Player's available money

	// Bets
	int bet_;
	int split_bet_;
	int* current_bet_ = &bet_;

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