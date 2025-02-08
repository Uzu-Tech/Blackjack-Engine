#include "deck.h"
#include <random>
#include <algorithm>

std::string Card::getRankAsString() const {
    switch (rank) {
    case Card::Rank::KING: return "K";
    case Card::Rank::QUEEN: return "Q";
    case Card::Rank::JACK: return "J";
    case Card::Rank::ACE: return "A";
    // For numeric ranks (2-10), we convert the integer to string
    default: return std::to_string(static_cast<int>(rank));
    }
}

int Card::rankToValue(Card::Rank rank) {
    const int FACE_CARD_VALUE = 10;  // Static constant
    const int ACE_CARD_VALUE = 11;   // Static constant

    switch (rank) {
        // Face Cards
    case Card::Rank::KING:
    case Card::Rank::QUEEN:
    case Card::Rank::JACK:
        return FACE_CARD_VALUE;
    case Card::Rank::ACE:
        return ACE_CARD_VALUE;
    default:
        return static_cast<int>(rank);
    }
}


void Deck::buildDeck() {
    deck_.clear();
    deck_.reserve(DECK_SIZE);

    for (int i = 0; i < Rules::NUM_DECKS; i++) {
        // Fill each deck with every suit and rank combo
        for (int suit = static_cast<int>(Card::Suit::CLUB);
            suit <= static_cast<int>(Card::Suit::DIAMOND);
            suit++) {

            for (int rank = static_cast<int>(Card::Rank::TWO);
                rank <= static_cast<int>(Card::Rank::ACE);
                rank++) {

                deck_.emplace_back(
                    static_cast<Card::Rank>(rank), static_cast<Card::Suit>(suit)
                );
            }
        }
    }
}

void Deck::shuffleDeck()
{
    std::random_device rd;  // Get a random seed
    std::mt19937 g(rd());   // Seed Mersenne Twister PRNG

    std::shuffle(deck_.begin(), deck_.end(), g); // Shuffle using g
}


Card Deck::drawCard() {
    if (deck_.empty()) {
        throw std::out_of_range("Cannot draw from an empty deck");
    }

    Card card = deck_.back();
    deck_.pop_back();
    return card;
}


void Deck::riffleShuffleDeck() {
    if (deck_.size() != Deck::DECK_SIZE * Rules::NUM_DECKS) {
        throw std::runtime_error("Deck should be full before riffle shuffling");
    }

    std::queue<Card> card_queue;
    size_t top_half_idx = deck_.size() / 2;

    for (int i = 0; i < deck_.size(); i++) {
        // Even indices
        if (i % 2 == 0 && !card_queue.empty()) {
            // Add the stored card
            if (i < deck_.size() / 2)
                card_queue.push(deck_[i]);
            deck_[i] = card_queue.front();
            card_queue.pop();
        }
        // Odd indices 
        else if (i % 2 == 1) {
            // Top half indices are already moved in deck
            if (i < deck_.size() / 2)
                card_queue.push(deck_[i]);
            deck_[i] = deck_[top_half_idx];
            top_half_idx++;
        }
    }
}