#include "player.h"

std::string GET_UPGRADE_NAME(Upgrades u) {
	switch (u) {
	case ADD_ATTEMPT: return "Add Attempt";
	case MONEY_END: return "Money End";
	case X_WORD: return "X Word";
	case MONEY_MULTIPLIER: return "Money Multiplier";
	case INVINCIBILITY: return "Invincibility";
	default: return "Unknown Upgrade";
	}
}