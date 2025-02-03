#pragma once
#include "deck.h"
#include <iostream>

class Hand {
public:
	static constexpr int NUM_INITAL_CARDS = 2;
	static constexpr int MAX_NUM_CARDS = 11; // 4 Aces, 4 Twos and 3 Threes = 21
	static constexpr int MAX_DECK_HAND = 17;
	// Contructors
	virtual ~Hand() {};
	Hand() { hand_.reserve(MAX_NUM_CARDS); };

	void dealHand(Deck& deck);
	int getHandValue();
	void clearHand();
	void hitHand(Deck& deck);
	bool isSoftHand() const { return is_hand_soft_; };

	virtual void displayHand() = 0;

protected:
	std::vector<Card> hand_;
	bool is_hand_soft_ = false;
};

class Dealer : public Hand {
public:
	enum class Action { HIT, STAND };
	Dealer::Action decideAction();
	void displayHand() override;
};