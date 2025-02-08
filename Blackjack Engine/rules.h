#pragma once

namespace Rules {
	constexpr int NUM_DECKS = 6;
	constexpr int NUM_RIFFLE_SHUFFLES = 3;
	constexpr float RESHUFFLE_THRESHOLD = 0.25; // Reshuffle when only 25% of cards are left
}

namespace Settings {
	constexpr bool DISPLAY_OFF = false;
}