#pragma once

#include "constants.h"
#include <unordered_set>

enum Upgrades {
  ADD_ATTEMPT,
  MONEY_END,
  X_WORD,
  MONEY_MULTIPLIER,
  INVINCIBILITY,
  SECOND_WORD_MULT,
  REVEAL_YELLOW,
};

std::string GET_UPGRADE_NAME(Upgrades u);

struct Player {
  // Upgrades that the player has unlocked
  std::unordered_multiset<Upgrades> upgrades;
  size_t money = 0;
};
