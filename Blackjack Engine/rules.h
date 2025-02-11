#pragma once

namespace Rules {
	constexpr int NUM_DECKS = 6;
	constexpr float RESHUFFLE_THRESHOLD = 0.25; // Reshuffle when only 25% of cards are left
}

namespace Settings {
	constexpr long long BANKROLL = 1'000'000'000;
	constexpr long long NUM_SIMULATIONS = 1'000'000;
	constexpr bool DISPLAY_OFF = true;
	constexpr int PRECISION = 3;
}