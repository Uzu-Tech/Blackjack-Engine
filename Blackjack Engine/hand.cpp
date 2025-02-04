#include "hand.h"

// Hand Methods
void Hand::dealHand(Deck& deck)
{
	for (int i = 0; i < Hand::NUM_INITAL_CARDS; i++) {
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
	while (hand_value > Hand::BLACKJACK && num_aces_ > 0) {
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

	return (hand_value <= Hand::BLACKJACK && num_aces_ > 0);
}

bool Hand::isPairHand() const
{
	if (hand_.empty()) {
		throw std::runtime_error("Cannot calculate if hand is a pair from empty hand");
	}

	return (hand_.size() == 2 && hand_[0].rank == hand_[1].rank);
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

Player::Action Player::decideAction(const Card& upcard) const
{

	return Player::Action();
}

Player::Action Player::generateHardHandLookupTable() const
{
	return Action();
}


Player::Action Player::generateSoftHandLookupTable() const
{
	return Action();
}


Player::Action Player::generatePairHandLookupTable() const
{
	return Action();
}


void Player::displayAction(Player::Action action) const
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

void Player::displayHand() const
{
	if (hand_.empty()) {
		throw std::runtime_error("Cannot display empty hand");
	}

	std::cout << "Player Hand: ";

	for (const Card& card : hand_) {
		std::cout << card.getRankAsString() << " ";
	}

	std::cout << "\nHand Value: " << getHandValue() << "\n\n";
}

void Player::displayBetAndBankroll() const
{
	std::cout << "Bankroll: " << bankroll_ << std::endl;
	std::cout << "Bet: " << bet_ << "\n\n";
}