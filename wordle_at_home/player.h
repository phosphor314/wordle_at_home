#pragma once

#include <unordered_set>
#include "constants.h"

enum Upgrades {
	ADD_ATTEMPT,
	MONEY_END,
	X_WORD,
	MONEY_MULTIPLIER,
	INVINCIBILITY,

};


std::string GET_UPGRADE_NAME(Upgrades u);


struct Player{
	// Upgrades that the player has unlocked
	std::unordered_multiset<Upgrades> upgrades = { ADD_ATTEMPT, MONEY_MULTIPLIER, INVINCIBILITY };
	size_t money = 0; 
};
