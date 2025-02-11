#include "game.h"
constexpr bool HIDDEN_UPCARD = true;

// TODO check blackjack problem, fix increment hand

void playBlackjackGame(Deck& deck, Player& player, Dealer& dealer)
{
	int reshuffle_threshold = static_cast<int>(std::round(Deck::DECK_SIZE * Rules::NUM_DECKS * Rules::RESHUFFLE_THRESHOLD));

	while (deck.size() > reshuffle_threshold) {
		player.calculateBet();
		player.displayBetAndBankroll();
		playBlackjackRound(deck, player, dealer);

		if (!Settings::DISPLAY_OFF)
		{
			std::cin.get();
			system("cls");
		}
	}

	if (!Settings::DISPLAY_OFF)
	{
		std::cout << "Round Over";
	}
}

void playBlackjackRound(Deck& deck, Player& player, Dealer& dealer)
{
	// Deal initial hands
	dealer.dealHand(deck);
	player.dealHand(deck);
	// Display initial hands
	dealer.displayHand(HIDDEN_UPCARD);
	player.displayHand();
	// Player loop
	bool next_hand_empty = false;
	bool has_blackjack_or_bust = true;

	while (!next_hand_empty) {
		Player::Outcome player_outcome = playHand(deck, player, dealer);
		next_hand_empty = player.isNextHandEmpty();
		// Move on to next hand if it hasn't been played yet
		if (player_outcome != Player::Outcome::SPLIT && !next_hand_empty) {
			player.incrementHand();
		}
		// If any of the hands don't bust or have blackjack we have to view the dealer's hand
		has_blackjack_or_bust = (hasBlackjackOrBust(player, player_outcome) && has_blackjack_or_bust);
	}

	player.returnToNormalHand();
	// Show dealer upcard
	dealer.displayHand(!HIDDEN_UPCARD);
	// Dealer loop only if we need to
	if (!has_blackjack_or_bust) {
		dealerLoop(dealer, deck);
	}
	// Display results
	int dealer_hand_value = dealer.getHandValue();
	for (int i = 0; i < HandConstants::MAX_NUM_HANDS; i++) {
		Player::Result result = player.updateBankroll(dealer_hand_value, dealer.hasBlackjack());
		player.displayResult(result);

		if (player.isNextHandEmpty()) {
			break;
		}
		else {
			player.incrementHand();
		}
	}

	player.reset();
	dealer.reset();
}

Player::Outcome playHand(Deck& deck, Player& player, Dealer& dealer)
{
    // Player loop
    bool can_hit = true;
	Card upcard = dealer.getUpcard();
	int num = 0;
    while (can_hit) {
		// Check for blackjack (first check since we could have blackjack straight away)
		if (player.hasBlackjack()) {
			return Player::Outcome::BLACKJACK;
		}
        // Decide Action, takes into account if we've split three times
        Player::Action player_action = player.decideAction(upcard);

		// If hand is double or stand and we can't double then we hit or stand
		if (player.getHandLength() != HandConstants::NUM_CARDS_FOR_DOUBLE &&
			(player_action == Player::Action::DOUBLE || player_action == Player::Action::DOUBLE_OR_STAND)) {
			player_action = (player_action == Player::Action::DOUBLE) ? Player::Action::HIT : Player::Action::STAND;
		}

        player.displayAction(player_action);

		switch (player_action) {

		case Player::Action::STAND:
			return Player::Outcome::STAND;

		case Player::Action::SPLIT:
			player.splitHand(deck);
			return Player::Outcome::SPLIT;

		case Player::Action::DOUBLE_OR_STAND:
		case Player::Action::DOUBLE:
			player.doubleDown(deck);
			can_hit = false;
			break;

		case Player::Action::HIT:
			player.hitHand(deck);
			break;

		default:
			break;
		}

		player.displayHand();

        // Bust or Blackjack we stop running
        if (player.isBust()) {
            return Player::Outcome::BUST;
        }
    }

	// If we've doubled and haven't busted or blackjack, we stand
	return Player::Outcome::STAND;
}

void dealerLoop(Dealer& dealer, Deck& deck) {
	Dealer::Action dealer_action;
	while ((dealer_action = dealer.decideAction()) == Dealer::Action::HIT) {
		dealer.displayAction(dealer_action);
		dealer.hitHand(deck);
		dealer.displayHand(!HIDDEN_UPCARD);
	}
	// Display stands if dealer does not bust or has blackjack
	if (!dealer.isBust() && !dealer.hasBlackjack()) {
		dealer.displayAction(Dealer::Action::STAND);
	}
}

bool hasBlackjackOrBust(Player& player, Player::Outcome outcome) {
	return (player.isBust() || player.hasBlackjack());
}
