#include "player.h"

// Player Methods
void Player::calculateBet()
{
	bets_[0] = 100;
	total_bet_ += bets_[0];
}

Player::Action Player::decideAction(const Card& upcard) const
{
	if (hands_[hand_idx_].isEmpty()) {
		throw std::runtime_error("Cannot decide action from empty hand");
	}

	int hand_value = hands_[hand_idx_].getHandValue();
	// Shift upcard value to match the lookup table indexes
	int upcard_lookup = Card::rankToValue(upcard.rank) - HandConstants::MIN_DEALER_UPCARD;

	if (hands_[hand_idx_].isPairHand() && num_splits_ < HandConstants::MAX_NUM_HANDS - 1) {
		int pair_hand_value = Card::rankToValue(hands_[hand_idx_].getHand().front().rank);
		return pair_hand_lookup_[pair_hand_value - HandConstants::MIN_PAIR_HAND][upcard_lookup];
	}

	if (hands_[hand_idx_].isSoftHand()) {
		return soft_hand_lookup_[hand_value - HandConstants::MIN_SOFT_HAND][upcard_lookup];
	}

	if (hand_value - HandConstants::MIN_HARD_HAND >= HandConstants::NUM_HARD_HANDS) {
		throw std::runtime_error("Cannot decide action for unrecognized hand value");
	}

	return hard_hand_lookup_[hand_value - HandConstants::MIN_HARD_HAND][upcard_lookup];
}

void Player::doubleDown(Deck& deck)
{
	if (hands_[hand_idx_].getHand().size() > 2) {
		throw std::runtime_error("Cannot double down with more than two cards");
	}

	bets_[bet_idx_] *= 2; // Double the bet
	hitHand(deck); // Hit one more card
}

void Player::splitHand(Deck& deck)
{
	if (hands_[hand_idx_].isEmpty()) {
		throw std::runtime_error("Cannot split empty hand");
	}

	if (hands_[hand_idx_].getHand().size() != 2) {
		throw std::runtime_error("Cannot split hand with more or less than two cards");
	}

	if (hands_[hand_idx_].getHand().front().rank != hands_[hand_idx_].getHand().back().rank) {
		throw std::runtime_error("Cannot split hand with different cards");
	}

	if (num_splits_ >= 3) {
		throw std::runtime_error("You can only split a maximum of 3 times");
	}

	size_t new_hand_idx = hand_idx_;
	while (!hands_[new_hand_idx].isEmpty() && new_hand_idx < HandConstants::MAX_NUM_HANDS) {
		new_hand_idx++;
	}

	if (new_hand_idx == HandConstants::MAX_NUM_HANDS) {
		throw std::runtime_error("At least one hand must be empty before splitting");
	}

	if (new_hand_idx >= HandConstants::MAX_NUM_HANDS) {
		throw std::runtime_error("Cannot split hand, maximum number of hands reached");
	}
	else {
		hands_[new_hand_idx].addCard(hands_[hand_idx_].getHand().front());
		// Split the hand by adding to the split hand and removing it from the current hand
		hands_[hand_idx_].removeCard();
		bets_[new_hand_idx] = bets_[0]; // Set the split bet
		num_splits_++;
	}
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
				hard_hand_lookup_[hand_lookup][upcard_lookup] = (
					upcard_value < 10 ?
					Player::Action::DOUBLE : Player::Action::HIT
					);
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
			
			//else if (hand_value == 19) {
			//	soft_hand_lookup_[hand_lookup][upcard_lookup] = (
			//		upcard_value == 6 ?
			//		Player::Action::DOUBLE_OR_STAND : Player::Action::STAND
			//		);
			//}
			
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
			// Ace
			else {
				Player::Action::SPLIT;
			}
		}
	}
}

Player::Result Player::updateBankroll(int dealer_value, bool dealerHasBlackjack)
{
	int hand_value = hands_[hand_idx_].getHandValue();
	num_hands_++;

	if (hand_value > HandConstants::BLACKJACK) {
		bankroll_ -= bets_[bet_idx_];
		num_losses_++;
		return Player::Result::BUST;
	}
	else if (hasBlackjack()) {
		bankroll_ += static_cast<long long>(bets_[bet_idx_] * 1.5);
		num_blackjacks_++;
		return Player::Result::BLACKJACK;
	}
	else if (hand_value > dealer_value || dealer_value > HandConstants::BLACKJACK) {
		bankroll_ += bets_[bet_idx_];
		num_wins_++;
		return (dealer_value > HandConstants::BLACKJACK) ? 
				Player::Result::DEALER_BUST : Player::Result::WIN;
	}
	else if (hand_value < dealer_value || dealer_value == HandConstants::BLACKJACK) {
		bankroll_ -= bets_[bet_idx_];
		num_losses_++;
		return (dealerHasBlackjack) ?
			Player::Result::DEALER_BLACKJACK : Player::Result::LOSE;
	}
	return Player::Result::TIE;
}

void Player::reset()
{
	for (int i = 0; i < HandConstants::MAX_NUM_HANDS; i++) {
		hands_[i].clearHand();
		bets_[bet_idx_] = 0;
	}

	num_splits_ = 0;
	hand_idx_ = 0;
	bet_idx_ = 0;
}

void Player::displayBetAndBankroll() const
{
	if (Settings::DISPLAY_OFF) { return; }
	std::cout << "Bankroll: " << bankroll_ << std::endl;
	std::cout << "Bet: " << bets_[0] << "\n\n";
}

void Player::displayStats() const
{
	double win_percentage = (double)num_wins_ / num_hands_;
	double loss_percentage = (double)num_losses_ / num_hands_;
	double blackjack_percenatge = (double)num_blackjacks_ / num_hands_;
	double expected_value = (double)getNetEarnings() / total_bet_;

	std::cout << "Net Earnings: " << getNetEarnings() << "\n";
	std::cout << "Expected Value: " << expected_value << "\n";
	std::cout << "Win Percenatage: " << win_percentage << "\n";
	std::cout << "Lose Percenatage: " << loss_percentage << "\n";
	std::cout << "Blackjack Percenatage: " << blackjack_percenatge << "\n";
}

void Player::displayHand() const
{
	if (Settings::DISPLAY_OFF) { return; }

	for (int i = 0; i < HandConstants::MAX_NUM_HANDS; i++) {
		if (hands_[i].isEmpty()) {
			break;
		}
		// If on the split hand display the extra information
		if (i != 0) {
			std::cout << "Player Hand " << i + 1 << ": ";
		}
		else {
			std::cout << "Player Hand: ";
		}

		for (const Card& card : hands_[i].getHand()) {
			std::cout << card.getRankAsString() << " ";
		}

		std::cout << "\nHand Value: " << hands_[i].handValueToString();
	}
	std::cout << "\n\n";
}

void Player::displayAction(Player::Action action) const
{
	if (Settings::DISPLAY_OFF) { return; }
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

void Player::displayResult(Player::Result result) const {
	if (Settings::DISPLAY_OFF) { return; }
	static const std::unordered_map<Player::Result, std::string> resultMessages = {
		{Player::Result::BLACKJACK, "Player Blackjack!!"},
		{Player::Result::BUST, "Player Busts"},
		{Player::Result::WIN, "Player Wins"},
		{Player::Result::LOSE, "Player Loses"},
		{Player::Result::TIE, "Player Ties"},
		{Player::Result::DEALER_BLACKJACK, "Dealer Blackjack"},
		{Player::Result::DEALER_BUST, "Dealer Busts"}
	};

	// Display hand number if playing on split hand
	if (hand_idx_ > 0) {
		std::cout << "(Hand " << hand_idx_ << ") ";
	}

	// Fetch message from map
	auto message = resultMessages.find(result);
	std::cout << message->second;
	std::cout << "\n";
}