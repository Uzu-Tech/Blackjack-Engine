#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <queue>

struct Card {
    static constexpr int ACE_ALTERNATIVE_VALUE = 1;

    enum class Rank {
        TWO = 2, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, KING, QUEEN, ACE
    };

    enum class Suit {
        CLUB, SPADE, HEART, DIAMOND
    };

    Rank rank;
    Suit suit;
    // Constructor
    Card(Rank r, Suit s) : rank(r), suit(s) {}
    std::string getRankAsString() const;
    static int rankToValue(Card::Rank rank);
};

class Deck {
public:
    static constexpr int DECK_SIZE = 52;

    Deck() { buildDeck(); };
    void buildDeck();
    void shuffleDeck();
    std::vector<Card> getDeck() { return deck_; };
    Card drawCard();
    void riffleShuffleDeck();

private:
    std::vector<Card> deck_;
};
