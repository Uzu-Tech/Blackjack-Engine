#include "hand.h"

// Hand Methods
void Hand::dealHand(Deck& deck)
{
	if (!hand_.empty()) {
		throw std::runtime_error("Hand must be empty before dealing two cards");
	}

	for (int i = 0; i < Hand::NUM_INITAL_CARDS; i++) {
		hand_.push_back(deck.drawCard());
	}
}

int Hand::getHandValue()
{
	if (hand_.empty()) {
		throw std::runtime_error("Cannot calculate hand value from empty hand");
	}

	int ace_count = 0, hand_value = 0;
	for (Card& card : hand_) {
		hand_value += Card::rankToValue(card.rank);
		if (card.getRankAsString() == "A") {
			ace_count++;
		}
	}

	while (hand_value > 21 && ace_count > 0) {
		hand_value -= (Card::rankToValue(Card::Rank::ACE) - Card::ACE_ALTERNATIVE_VALUE);
		ace_count--;
	}

	is_hand_soft_ = (ace_count > 0);
	return hand_value;
}

void Hand::clearHand()
{
	hand_.clear();
}

void Hand::hitHand(Deck& deck)
{
	if (hand_.empty()) {
		throw std::runtime_error("Can't hit an empty hand");
	}

	hand_.push_back(deck.drawCard());
}

// Dealer Methods
Dealer::Action Dealer::decideAction()
{
	int hand_value = getHandValue();
	return (
		(hand_value < Dealer::MAX_DECK_HAND || (isSoftHand() && hand_value == Dealer::MAX_DECK_HAND))?
		Dealer::Action::HIT : Dealer::Action::STAND
	);
}

void Dealer::displayHand()
{
	std::cout << "Dealer Hand:" << std::endl;

	for (Card& card : hand_) {
		std::cout << card.getRankAsString() << " ";
	}
	std::cout << std::endl << std::endl;
}
