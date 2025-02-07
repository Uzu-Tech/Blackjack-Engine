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
	Card card = deck.drawCard();
	hand_.push_back(card);
}

int Hand::getHandValue() const
{
	if (hand_.empty()) {
		throw std::runtime_error("Cannot calculate hand value from empty hand");
	}

	int hand_value = 0, ace_count = 0;
	for (const Card& card : hand_) {
		hand_value += Card::rankToValue(card.rank);
		if (card.rank == Card::Rank::ACE) {
			ace_count++;
		}
	}

	while (hand_value > HandConstants::BLACKJACK && ace_count > 0) {
		// Change the value of the Ace from 11 to 1
		hand_value -= (Card::rankToValue(Card::Rank::ACE) - Card::ACE_ALTERNATIVE_VALUE);
		ace_count--;
	}

	return hand_value;
}

void Hand::clearHand()
{
	hand_.clear();
}

bool Hand::isSoftHand() const
{
	if (hand_.empty()) {
		throw std::runtime_error("Cannot calculate if hand is soft from empty hand");
	}

	int hand_value = 0, ace_count = 0;
	for (const Card& card : hand_) {
		hand_value += Card::rankToValue(card.rank);
		if (card.rank == Card::Rank::ACE) {
			ace_count++;
		}
	}

	while (hand_value > HandConstants::BLACKJACK && ace_count > 0) {
		// Change the value of the Ace from 11 to 1
		hand_value -= (Card::rankToValue(Card::Rank::ACE) - Card::ACE_ALTERNATIVE_VALUE);
		ace_count--;
	}

	return (hand_value <= HandConstants::BLACKJACK && ace_count > 0);
}

bool Hand::isPairHand() const
{
	if (hand_.empty()) {
		throw std::runtime_error("Cannot calculate if hand is a pair from empty hand");
	}

	return (hand_.size() == 2 && hand_[0].rank == hand_[1].rank);
}

std::string Hand::handValueToString() const
{
	int hand_value = getHandValue();

	if (hand_value > HandConstants::BLACKJACK) {
		return "Bust";
	}
	else {
		return std::to_string(getHandValue());
	}
}
