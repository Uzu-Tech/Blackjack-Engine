#include <iostream>
#include "deck.h"
#include "hand.h"
#include "player.h"
#include "dealer.h"

#define HIDDEN_UPCARD true
//#include <chrono>
//using namespace std::chrono;
//auto start = high_resolution_clock::now();
//auto end = high_resolution_clock::now();
//auto duration = duration_cast<microseconds>(end - start);

// TODO - Chnage display so it matches with hit and double down

int main()
{
	constexpr int BANKROLL = 10000;
	const Card::Suit SUIT = Card::Suit::CLUB;

    Deck deck;
    deck.shuffleDeck();

    Dealer dealer;
    Player player(BANKROLL);
    dealer.dealHand(deck);
    player.setHand(Card(Card::Rank::TWO, SUIT), Card(Card::Rank::TWO, SUIT));

    player.calculateBet(5, 1000);
    player.displayBetAndBankroll();

    dealer.displayHand();
    player.displayHand();
    bool running = true;

    while (running) {
		// Decide Action
        Player::Action player_action = player.decideAction(dealer.getUpcard());
		player.displayAction(player_action);

        // Perform Action
        switch (player_action) {
        case Player::Action::HIT:
			player.hitHand(deck);
			break;
        case Player::Action::DOUBLE_OR_STAND:
		case Player::Action::DOUBLE:
            if (player.getHandLength() == 2) {
			    player.doubleDown(deck);
			}
            // Stop running if we can't double on a double or stand
			else if (player_action == Player::Action::DOUBLE_OR_STAND) {
                running = false;
            }
            // If it's a reguale double and we cna't double then hit
            else {
                player.hitHand(deck);
            }
            // Only double down once
			running = false;
            break;
        case Player::Action::SPLIT:
            player.splitHand(deck);
            break;

		case Player::Action::STAND:
            // Don't stop running if we need to switch hands
            running = false;
			break;
		default:
			break;
        }

        // Bust or Blackjack we stop running
		if (player.isBust() || player.hasBlackjack()) {
			running = false;
		}

		// Continue running if we have split hands
		if (!running && player.hasSplit() && !player.isOnSplitHand()) {
			player.changeToSplitHand();
            running = true;
		}

        // No need to show hand again if stands
        if (player_action != Player::Action::STAND) {
            player.displayHand();
        }
    }

    std::cout << "-------------------------\n\n";

    Dealer::Action dealer_action;
    while ((dealer_action = dealer.decideAction()) == Dealer::Action::HIT) {
        dealer.displayHand(!HIDDEN_UPCARD);
        dealer.displayAction(dealer_action);
        dealer.hitHand(deck);
    }
    dealer.displayHand(!HIDDEN_UPCARD);
}