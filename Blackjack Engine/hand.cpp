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

void Dealer::displayHand(bool hidden)
{
	if (hand_.empty()) {
		throw std::runtime_error("Cannot display empty hand");
	}

	std::cout << "Dealer Hand: ";

	if (!hidden) {
		// Display all cards
		for (Card& card : hand_) {
			std::cout << card.getRankAsString() << " ";
		}
	}
	else {
		// Display only the first card and hide the second
		std::cout << hand_[0].getRankAsString() << " ";
		std::cout << "??";
	}
	std::cout << "\n\n";
}


// Player Methods
void Player::calculateBet(int min_bet, int max_bet)
{
	bet_ = std::ceil(bankroll_ * 0.01);
}

Player::Action Player::decideAction(const Card& upcard)
{

	return Player::Action();
}

void Player::displayAction(Player::Action action)
{
	switch (action) {
	case Player::Action::HIT:
		std::cout << "Player hits\n\n";
		break;
	case Player::Action::STAND:
		std::cout << "Player stands\n\n";
		break;
	case Player::Action::DOUBLE:
		std::cout << "Player doubles down\n\n";
		break;
	case Player::Action::SPLIT:
		std::cout << "Player splits\n\n";
		break;
	}
}

void Player::updateBankroll(Player::Outcome outcome)
{
	switch (outcome) {
	case Player::Outcome::BLACKJACK:
		bankroll_ += std::round(bet_ * 1.5);
		break;

	case Player::Outcome::WIN:
		bankroll_ += bet_;
		break;

	case Player::Outcome::LOSE:
		bankroll_ -= bet_;
		break;
	}
}

void Player::displayHand()
{
	if (hand_.empty()) {
		throw std::runtime_error("Cannot display empty hand");
	}

	std::cout << "Player Hand: ";

	for (Card& card : hand_) {
		std::cout << card.getRankAsString() << " ";
	}

	std::cout << "\nHand Value: " << getHandValue() << "\n\n";
}

void Player::displayBetAndBankroll() const
{
	std::cout << "Bankroll: " << bankroll_ << std::endl;
	std::cout << "Bet: " << bet_ << "\n\n";
}

Player::Action Player::normalHandStrategy_(const Card& upcard)
{
	return Player::Action();
}

Player::Action Player::softHandStrategy_(const Card& upcard)
{
	return Player::Action();
}

Player::Action Player::pairHandStrategy_(const Card& upcard)
{
	return Player::Action();
}
