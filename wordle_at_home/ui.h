#pragma once

#include <SFML/Graphics.hpp>
#include "constants.h"
#include "player.h"
#include "layout.h"

namespace UI {
	void renderUpgradeBar(sf::RenderTarget* target, Constants& constants, Player& player, Layout::Layout& layout);
}
