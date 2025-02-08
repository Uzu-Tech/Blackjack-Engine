#include "game.h"
constexpr bool HIDDEN_UPCARD = true;

void playBlackjackGame(Deck& deck, Player& player, Dealer& dealer)
{
	int max_bankroll = player.getBankroll();
	for (int i = 0; i < Rules::NUM_RIFFLE_SHUFFLES; i++) {
		deck.riffleShuffleDeck();
	}

	int reshuffle_threshold = static_cast<int>(std::round(Deck::DECK_SIZE * Rules::NUM_DECKS * Rules::RESHUFFLE_THRESHOLD));

	while (deck.size() > reshuffle_threshold) {
		player.calculateBet(max_bankroll);
		player.displayBetAndBankroll();
		playBlackjackRound(deck, player, dealer);
		std::cin.get();
		system("cls");
	}

	std::cout << "Round Over";
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
	Player::Outcome player_outcome = playHand(deck, player, dealer);
	bool has_blackjack_or_bust = hasBlackjackOrBust(player, player_outcome);

	// Split loop
	if (player_outcome == Player::Outcome::SPLIT) {
		Player::Outcome outcome1 = playHand(deck, player, dealer);
		bool hand1_bj_or_bust = hasBlackjackOrBust(player, player_outcome);

		player.switchToSplitHand();
		Player::Outcome outcome2 = playHand(deck, player, dealer);
		bool hand2_bj_or_bust = hasBlackjackOrBust(player, player_outcome);
		// Return to normal hand for displaying
		player.switchToNormalHand();

		has_blackjack_or_bust = (hand1_bj_or_bust && hand2_bj_or_bust);
	}

	// Show dealer upcard
	dealer.displayHand(!HIDDEN_UPCARD);
	// Dealer loop
	dealerLoop(dealer, deck, has_blackjack_or_bust);
	// Display results
	Player::Result result = player.updateBankroll(dealer.getHandValue());
	player.displayResult(result);
	// Repeat for the split hand if needed
	if (player.hasSplit()) {
		player.switchToSplitHand();
		result = player.updateBankroll(dealer.getHandValue());
		player.displayResult(result);
	}
	player.reset();
	dealer.reset();
}

Player::Outcome playHand(Deck& deck, Player& player, Dealer& dealer)
{
    // Player loop
    bool can_hit = true;
	Card upcard = dealer.getUpcard();
    while (can_hit) {
		// Check for blackjack (first check since we could have blackjack straight away)
		if (player.hasBlackjack()) {
			return Player::Outcome::BLACKJACK;
		}
        // Decide Action
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

void dealerLoop(Dealer& dealer, Deck& deck, bool has_blackjack_or_bust) {
	// No need to play dealer hand
	if (has_blackjack_or_bust) {
		return;
	}

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
