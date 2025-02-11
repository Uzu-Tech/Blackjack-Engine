#pragma once
#include "deck.h"
#include "hand.h"
#include <unordered_map>
#include <cmath>
#include <array>

class Player {
public:
	// Possible actions for the player
	enum class Action { HIT, STAND, DOUBLE, SPLIT, DOUBLE_OR_STAND };
	// Possible outcomes for the player
	enum class Outcome { BLACKJACK, BUST, STAND, SPLIT };
	// Results for displaying to terminal
	enum class Result { BLACKJACK, BUST, WIN, LOSE, TIE, DEALER_BUST, DEALER_BLACKJACK };

	// Player starts with zero bet
	Player(int bankroll) : bankroll_(bankroll), initial_bankroll_(bankroll), total_bet_(0),
		hard_hand_lookup_{}, soft_hand_lookup_{}, pair_hand_lookup_{} {
		// Generate strategy lookup tables
		generateHardHandLookupTable_();
		generateSoftHandLookupTable_();
		generatePairHandLookupTable_();

		for (int i = 0; i < HandConstants::MAX_NUM_HANDS; i++) {
			hands_[i] = Hand();
			bets_[i] = 0;
		}

	};

	// Hand methods
	void dealHand(Deck& deck) { hands_[hand_idx_].dealHand(deck); };
	// Debugging method to set the player's hand
	void setHand(const Card& card1, const Card& card2) {
		hands_[hand_idx_].addCard(card1);
		hands_[hand_idx_].addCard(card2);
	}
	void hitHand(Deck& deck) {
		hands_[hand_idx_].hitHand(deck); 
	};

	// Calculates the bet based on the player's strategy
	void calculateBet();
	// Implements a strategy to decide the player's action
	Action decideAction(const Card& upcard) const;
	// Actions
	void doubleDown(Deck& deck);
	void splitHand(Deck& deck);
	// Switch hands
	void incrementHand()
	{
		if (hand_idx_ >= hands_.size() - 1) {
			throw std::runtime_error("You can only have a max of four hands");
		}

		if (hands_[hand_idx_ + 1].isEmpty()) {
			throw std::runtime_error("You can't switch to an empty hand");
		}

		bet_idx_++;
		hand_idx_++;
	}
	void returnToNormalHand() {
		// First hand and bet is at index 0
		hand_idx_ = 0;
		bet_idx_ = 0;
	}
	// Outcome checks
	bool isBust() const { return hands_[hand_idx_].getHandValue() > HandConstants::BLACKJACK; };
	bool hasBlackjack() const { 
		return (
			hands_[hand_idx_].getHandValue() == HandConstants::BLACKJACK
			&& getHandLength() == 2 // Blackjack only when has an Ace or King
		);
	};
	// Displays the player's action to the console
	void displayAction(Action action) const;
	void displayResult(Player::Result result) const;
	// Updates the player's bankroll based on the outcome
	Player::Result updateBankroll(int dealer_hand_value, bool dealerHasBlackjack);
	void displayHand() const;
	void displayBetAndBankroll() const;

	void displayStats() const;

	void reset();

	// Getters
    bool isNextHandEmpty() const {
        if (hand_idx_ >= hands_.size() - 1) {
            return true;
        }
        return hands_[hand_idx_ + 1].isEmpty();
    }

	bool isHandEmpty() const { return hands_[hand_idx_].isEmpty();  }
	long long getBankroll() const { return bankroll_; };
	int getBet() const { return bets_[bet_idx_]; };
	bool isBroke() const { return bankroll_ == 0; };
	size_t getHandLength() const { return hands_[hand_idx_].getHand().size(); }

	int getWins() const { return num_wins_; };
	int getBlackjacks() const { return num_blackjacks_; };
	int getLosses() const { return num_losses_; };
	long long getNetEarnings() const { return bankroll_ - initial_bankroll_; };
	long long getTotalBet() const { return total_bet_; };

private:
	long long bankroll_; // Player's available money
	long long initial_bankroll_;
	// Bets
	std::array<int, HandConstants::MAX_NUM_HANDS> bets_;
	size_t bet_idx_ = 0;
	long long total_bet_;

	// Hands
	std::array<Hand, HandConstants::MAX_NUM_HANDS> hands_;
	size_t hand_idx_ = 0;
	int num_splits_ = 0;

	bool has_split_ = false;

	// Strategy lookup tables
	Action hard_hand_lookup_[HandConstants::NUM_HARD_HANDS][HandConstants::NUM_DEALER_UPCARDS];
	Action soft_hand_lookup_[HandConstants::NUM_SOFT_HANDS][HandConstants::NUM_DEALER_UPCARDS];
	Action pair_hand_lookup_[HandConstants::NUM_PAIR_HANDS][HandConstants::NUM_DEALER_UPCARDS];

	// Private strategy methods for the player
	void generateHardHandLookupTable_();
	void generateSoftHandLookupTable_();
	void generatePairHandLookupTable_();

	// Stats
	int num_wins_ = 0;
	int num_blackjacks_ = 0;
	int num_losses_ = 0;
	int num_hands_ = 0;
};