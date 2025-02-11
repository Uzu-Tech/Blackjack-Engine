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

	// Debugging method to set the dealer's hand
	void setHand(const Card& card1, const Card& card2) {
		hand_.addCard(card1);
		hand_.addCard(card2);
	}

	// Getters
	const Card& getUpcard() const { 
		if (hand_.isEmpty()) {
			throw std::runtime_error("Cannot get upcard from empty hand");
		}

		return hand_.getHand().front(); 
	};
	int getHandValue() const { return hand_.getHandValue(); }

	bool isBust() const { return hand_.getHandValue() > HandConstants::BLACKJACK; };
	bool hasBlackjack() const { 
		return (
			hand_.getHandValue() == HandConstants::BLACKJACK
			&& hand_.getHand().size() == 2 // Blackjack only when has an Ace or King
		);
	};

	void reset() { hand_.clearHand(); };

	// Auto plays the dealer's hand
	Action decideAction();
	void displayHand(bool hidden = true) const;
	void displayAction(Dealer::Action action) const;

private:
	Hand hand_ = Hand();
};

