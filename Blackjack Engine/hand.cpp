#include "hand.h"

// Hand Methods
void Hand::dealHand(Deck& deck)
{
	for (int i = 0; i < HandConstants::NUM_INITAL_CARDS; i++) {
		hitHand(deck);
	}
}

void Hand::hitHand(Deck& deck)
{
	if (hand_.empty()) {
		throw std::runtime_error("Can't deal card to empty hand");
	}

	Card card = deck.drawCard();
	if (card.rank == Card::Rank::ACE) {
		num_aces_++;
	}

	hand_.push_back(card);
}

int Hand::getHandValue() const
{
	if (hand_.empty()) {
		throw std::runtime_error("Cannot calculate hand value from empty hand");
	}

	int hand_value = 0;
	for (const Card& card : hand_) {
		hand_value += Card::rankToValue(card.rank);
	}

	int ace_count = num_aces_;
	while (hand_value > HandConstants::BLACKJACK && num_aces_ > 0) {
		// Change the value of the Ace from 11 to 1
		hand_value -= (Card::rankToValue(Card::Rank::ACE) - Card::ACE_ALTERNATIVE_VALUE);
		ace_count--;
	}

	return hand_value;
}

void Hand::clearHand()
{
	num_aces_ = 0;
	hand_.clear();
}

bool Hand::isSoftHand() const
{
	if (hand_.empty()) {
		throw std::runtime_error("Cannot calculate if hand is soft from empty hand");
	}

	int hand_value = 0;
	for (const Card& card : hand_) {
		hand_value += Card::rankToValue(card.rank);
	}

	return (hand_value <= HandConstants::BLACKJACK && num_aces_ > 0);
}

bool Hand::isPairHand() const
{
	if (hand_.empty()) {
		throw std::runtime_error("Cannot calculate if hand is a pair from empty hand");
	}

	return (hand_.size() == 2 && hand_[0].rank == hand_[1].rank);
}