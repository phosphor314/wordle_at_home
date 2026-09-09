#pragma once

#include "constants.h"
#include "layout.h"
#include "player.h"
#include <SFML/Graphics.hpp>

namespace UI {
	void renderUpgradeBar(sf::RenderTarget* target, Constants& constants, Player& player, Layout::Layout& layout);
}
