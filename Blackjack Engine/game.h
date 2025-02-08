#pragma once
#include "deck.h"
#include "hand.h"
#include "player.h"
#include "dealer.h"
#include "rules.h"
#include <cstdlib>

void playBlackjackGame(Deck& deck, Player& player, Dealer& dealer);
void playBlackjackRound(Deck& deck, Player& player, Dealer& dealer);
Player::Outcome playHand(Deck& deck, Player& player, Dealer& dealer);
void dealerLoop(Dealer& dealer, Deck& deck, bool has_blackjack_or_bust);
bool hasBlackjackOrBust(Player& player, Player::Outcome outcome);
