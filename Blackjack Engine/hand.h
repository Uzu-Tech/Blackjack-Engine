#pragma once
#include "deck.h"
#include <iostream>
#include <cmath>

namespace HandConstants {
	// General hand-related constants
	constexpr int NUM_INITAL_CARDS = 2; // Initial number of cards for a hand
	constexpr int MAX_NUM_CARDS = 11;   // 4 Aces, 4 Twos and 3 Threes = 21
	constexpr int MAX_DECK_HAND = 17;   // Dealer must hit on 16 or less
	constexpr int BLACKJACK = 21;       // Blackjack value

	// Min and max values for hand types
	constexpr int MIN_HARD_HAND = 2;
	constexpr int MAX_HARD_HAND = 20;

	constexpr int MIN_SOFT_HAND = 13;
	constexpr int MAX_SOFT_HAND = 20;

	constexpr int MIN_PAIR_HAND = 2;
	constexpr int MAX_PAIR_HAND = 11;

	constexpr int MIN_DEALER_UPCARD = 2;
	constexpr int MAX_DEALER_UPCARD = 11;

	// Dynamic sizes based on min/max values
	constexpr int NUM_HARD_HANDS = MAX_HARD_HAND - MIN_HARD_HAND + 1;
	constexpr int NUM_SOFT_HANDS = MAX_SOFT_HAND - MIN_SOFT_HAND + 1;
	constexpr int NUM_PAIR_HANDS = MAX_PAIR_HAND - MIN_PAIR_HAND + 1;
	constexpr int NUM_DEALER_UPCARDS = MAX_DEALER_UPCARD - MIN_DEALER_UPCARD + 1;
}

class Hand {
public:
	// Contructors and Destructors
	virtual ~Hand() {};
    Hand() { hand_.reserve(HandConstants::MAX_NUM_CARDS); };
	//Getter
	const std::vector<Card>& getHand() const { return hand_; };
	// Deals two cards to the hand
	void dealHand(Deck& deck);
	// Adds a random card to the hand
	void hitHand(Deck& deck);
	void addCard(const Card& card) { hand_.push_back(card); };
	void removeCard() { 
		if (hand_.empty()) {
			throw std::runtime_error("Cannot remove card from empty hand");
		}
		hand_.pop_back(); 
	};
	// Returns the value of the hand
	int getHandValue() const;
	void clearHand();
	// Whether the hand has an Ace valued as 11
	bool isSoftHand() const;
	bool isPairHand() const;
	bool isEmpty() const { return hand_.empty(); };
	// Hand value to string conversion
	std::string handValueToString() const;

private:
	std::vector<Card> hand_;
};