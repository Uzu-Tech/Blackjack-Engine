#include "dealer.h"

// Dealer Methods
Dealer::Action Dealer::decideAction()
{
	int hand_value = hand_.getHandValue();
	return (
		(hand_value < HandConstants::MAX_DECK_HAND) ?
		Dealer::Action::HIT : Dealer::Action::STAND
	);
}

void Dealer::displayHand(bool hidden) const
{
	if (Settings::DISPLAY_OFF) { return; }
	if (hand_.isEmpty()) {
		throw std::runtime_error("Cannot display empty hand");
	}

	std::cout << "Dealer Hand: ";

	if (!hidden) {
		// Display all cards
		for (const Card& card : hand_.getHand()) {
			std::cout << card.getRankAsString() << " ";
		}
		std::cout << "\nHand Value: " << hand_.handValueToString();
	}
	else {
		// Display only the first card and hide the second
		std::cout << hand_.getHand()[0].getRankAsString() << " ";
		std::cout << "??";
	}
	std::cout << "\n\n";
}

void Dealer::displayAction(Dealer::Action action) const 
{
	if (Settings::DISPLAY_OFF) { return; }
	switch (action) {
	case Dealer::Action::HIT:
		std::cout << "Dealer hits\n\n";
		break;
	case Dealer::Action::STAND:
		std::cout << "Dealer stands\n\n";
		break;
	}
}