#include "player.h"

std::string GET_UPGRADE_NAME(Upgrades u) {
	switch (u) {
	case ADD_ATTEMPT: return "Add Attempt";
	case MONEY_END: return "Money End";
	case X_WORD: return "X Word";
	case MONEY_MULTIPLIER: return "Money Multiplier";
	case INVINCIBILITY: return "Invincibility";
	case SECOND_WORD_MULT: return "Second Word Multiplier";
	case REVEAL_YELLOW: return "Yellow Letter";
	default: return "Unknown Upgrade";
	}
}