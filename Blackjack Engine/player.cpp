#include "player.h"

// Player Methods
void Player::calculateBet(int min_bet, int max_bet)
{
	bet_ = std::ceil(bankroll_ * 0.01);
}

Player::Action Player::decideAction(const Card& upcard) const
{
	if (current_hand_->isEmpty()) {
		throw std::runtime_error("Cannot decide action from empty hand");
	}

	int hand_value = current_hand_->getHandValue();
	// Shift upcard value to match the lookup table indexes
	int upcard_lookup = Card::rankToValue(upcard.rank) - HandConstants::MIN_DEALER_UPCARD;

	if (current_hand_->isPairHand() && !has_split_) {
		int pair_hand_value = Card::rankToValue(current_hand_->getHand().front().rank);
		return pair_hand_lookup_[pair_hand_value - HandConstants::MIN_PAIR_HAND][upcard_lookup];
	}

	if (current_hand_->isSoftHand()) {
		return soft_hand_lookup_[hand_value - HandConstants::MIN_SOFT_HAND][upcard_lookup];
	}

	return hard_hand_lookup_[hand_value - HandConstants::MIN_HARD_HAND][upcard_lookup];
}

void Player::doubleDown(Deck& deck)
{
	if (has_doubled_ && !has_split_) {
		throw std::runtime_error("Cannot double down twice");
	}

	if (current_hand_->getHand().size() > 2) {
		throw std::runtime_error("Cannot double down with more than two cards");
	}

	*current_bet_ *= 2; // Double the bet
	hitHand(deck); // Hit one more card
	has_doubled_ = true;
}

void Player::splitHand(Deck& deck)
{
	if (hand_.isEmpty()) {
		throw std::runtime_error("Cannot split empty hand");
	}

	if (hand_.getHand().size() != 2) {
		throw std::runtime_error("Cannot split hand with more or less than two cards");
	}

	if (hand_.getHand().front().rank != hand_.getHand().back().rank) {
		throw std::runtime_error("Cannot split hand with different cards");
	}

	if (!split_hand_.isEmpty()) {
		throw std::runtime_error("Split hand should be empty before splitting");
	}

	// Split the hand by adding to the split hand and removing it from the current hand
	split_hand_.addCard(hand_.getHand().front());
	split_bet_ = bet_; // Set the split bet
	hand_.removeCard();
	has_split_ = true;
}

void Player::generateHardHandLookupTable_()
{
	// Go through every possible hand value and dealer upcard value
	for (int hand_value = HandConstants::MIN_HARD_HAND;
		hand_value <= HandConstants::MAX_HARD_HAND;
		hand_value++)
	{
		for (int upcard_value = HandConstants::MIN_DEALER_UPCARD;
			upcard_value <= HandConstants::MAX_DEALER_UPCARD;
			upcard_value++)
		{
			// Shift the hand value and upcard value to match the lookup table indexes
			int hand_lookup = hand_value - HandConstants::MIN_HARD_HAND;
			int upcard_lookup = upcard_value - HandConstants::MIN_DEALER_UPCARD;

			if (hand_value < 9) {
				hard_hand_lookup_[hand_lookup][upcard_lookup] = Player::Action::HIT;
			}

			else if (hand_value == 9) {
				hard_hand_lookup_[hand_lookup][upcard_lookup] = (
					(upcard_value >= 3 && upcard_value <= 6) ?
					Player::Action::DOUBLE : Player::Action::HIT
					);
			}

			else if (hand_value == 10) {
				hard_hand_lookup_[hand_lookup][upcard_lookup] = (
					upcard_value < 10 ?
					Player::Action::DOUBLE : Player::Action::HIT
					);
			}

			else if (hand_value == 11) {
				hard_hand_lookup_[hand_lookup][upcard_lookup] = Player::Action::DOUBLE;
			}

			else if (hand_value == 12) {
				hard_hand_lookup_[hand_lookup][upcard_lookup] = (
					upcard_value >= 4 && upcard_value <= 6 ?
					Player::Action::STAND : Player::Action::HIT
					);
			}

			else if (hand_value <= 16) {
				hard_hand_lookup_[hand_lookup][upcard_lookup] = (
					upcard_value >= 7 ?
					Player::Action::HIT : Player::Action::STAND
					);
			}
			// 17 - 20
			else {
				hard_hand_lookup_[hand_lookup][upcard_lookup] = Player::Action::STAND;
			}

		}
	}
}


void Player::generateSoftHandLookupTable_()
{
	// Go through every possible soft value and dealer upcard value
	for (int hand_value = HandConstants::MIN_SOFT_HAND;
		hand_value <= HandConstants::MAX_SOFT_HAND;
		hand_value++)
	{
		for (int upcard_value = HandConstants::MIN_DEALER_UPCARD;
			upcard_value <= HandConstants::MAX_DEALER_UPCARD;
			upcard_value++)
		{
			// Shift the hand value and upcard value to match the lookup table indexes
			int hand_lookup = hand_value - HandConstants::MIN_SOFT_HAND;
			int upcard_lookup = upcard_value - HandConstants::MIN_DEALER_UPCARD;

			if (hand_value <= 14) {
				soft_hand_lookup_[hand_lookup][upcard_lookup] = (
					upcard_value >= 5 && upcard_value <= 6 ?
					Player::Action::DOUBLE : Player::Action::HIT
					);
			}

			else if (hand_value <= 16) {
				soft_hand_lookup_[hand_lookup][upcard_lookup] = (
					upcard_value >= 4 && upcard_value <= 6 ?
					Player::Action::DOUBLE : Player::Action::HIT
					);
			}

			else if (hand_value == 17) {
				soft_hand_lookup_[hand_lookup][upcard_lookup] = (
					upcard_value >= 3 && upcard_value <= 6 ?
					Player::Action::DOUBLE : Player::Action::HIT
					);
			}

			else if (hand_value == 18) {
				soft_hand_lookup_[hand_lookup][upcard_lookup] = (
					upcard_value >= 3 && upcard_value <= 6 ?
					Player::Action::DOUBLE_OR_STAND : (
						upcard_value >= 9 ?
						Player::Action::HIT : Player::Action::STAND
						)
					);
			}

			else if (hand_value == 19) {
				soft_hand_lookup_[hand_lookup][upcard_lookup] = (
					upcard_value == 6 ?
					Player::Action::DOUBLE_OR_STAND : Player::Action::STAND
					);
			}

			// Ace, 9
			else {
				soft_hand_lookup_[hand_lookup][upcard_lookup] = Player::Action::STAND;
			}
		}
	}
}


void Player::generatePairHandLookupTable_()
{
	for (int pair_value = HandConstants::MIN_PAIR_HAND;
		pair_value <= HandConstants::MAX_PAIR_HAND;
		pair_value++)
	{
		for (int upcard_value = HandConstants::MIN_DEALER_UPCARD;
			upcard_value <= HandConstants::MAX_DEALER_UPCARD;
			upcard_value++)
		{
			// Shift the hand value and upcard value to match the lookup table indexes
			int pair_lookup = pair_value - HandConstants::MIN_PAIR_HAND;
			int upcard_lookup = upcard_value - HandConstants::MIN_DEALER_UPCARD;

			if (pair_value <= 3) {
				pair_hand_lookup_[pair_lookup][upcard_lookup] = (
					upcard_value <= 7 ?
					Player::Action::SPLIT : Player::Action::HIT
					);
			}

			else if (pair_value == 4) {
				pair_hand_lookup_[pair_lookup][upcard_lookup] = (
					upcard_value >= 5 && upcard_value <= 6 ?
					Player::Action::SPLIT : Player::Action::HIT
					);
			}

			else if (pair_value == 5) {
				pair_hand_lookup_[pair_lookup][upcard_lookup] = (
					upcard_value >= 10 ?
					Player::Action::HIT : Player::Action::DOUBLE
				);
			}

			else if (pair_value == 6) {
				pair_hand_lookup_[pair_lookup][upcard_lookup] = (
					upcard_value <= 6 ?
					Player::Action::SPLIT : Player::Action::HIT
					);
			}

			else if (pair_value == 7) {
				pair_hand_lookup_[pair_lookup][upcard_lookup] = (
					upcard_value <= 7 ?
					Player::Action::SPLIT : Player::Action::HIT
					);
			}

			else if (pair_value == 8) {
				pair_hand_lookup_[pair_lookup][upcard_lookup] = Player::Action::SPLIT;
			}

			else if (pair_value == 9) {
				pair_hand_lookup_[pair_lookup][upcard_lookup] = (
					upcard_value == 7 || upcard_value == 10 || upcard_value == 11 ?
					Player::Action::STAND : Player::Action::SPLIT
					);
			}

			else if (pair_value == 10) {
				pair_hand_lookup_[pair_lookup][upcard_lookup] = Player::Action::STAND;
			}
		}
	}
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
	if (hand_.isEmpty()) {
		throw std::runtime_error("Cannot display empty hand");
	}

	// If on the split hand display the extra information
	std::cout << "Player Hand: ";

	for (const Card& card : hand_.getHand()) {
		std::cout << card.getRankAsString() << " ";
	}

	std::cout << "\nHand Value: " << hand_.handValueToString();

	if (has_split_) {
		std::cout << "\n\n\Player Hand 2: ";
		for (const Card& card : split_hand_.getHand()) {
			std::cout << card.getRankAsString() << " ";
		}

		std::cout << "\n\Hand Value: " << split_hand_.handValueToString();
	}

	std::cout << "\n\n";
}

void Player::displayBetAndBankroll() const
{
	std::cout << "Bankroll: " << bankroll_ << std::endl;
	std::cout << "Bet: " << bet_ << "\n\n";
}