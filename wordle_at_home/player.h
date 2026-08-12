#pragma once

#include <unordered_multiset>
#include "constants.h"

enum Upgrades {
	ADD_ATTEMPT,
	MONEY_END,
	X_WORD
};


struct Player{
	// Upgrades that the player has unlocked
	std::unordered_multiset<Upgrades> upgrades = { ADD_ATTEMPT };
	size_t money = 0; 
};
