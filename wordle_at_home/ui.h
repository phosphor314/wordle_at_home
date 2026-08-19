#pragma once

#include <SFML/Graphics.hpp>
#include "constants.h"
#include "player.h"
#include "layout.h"

namespace UI {
	// Render a horizontal upgrade bar at the top of the layout bounds.
	void renderUpgradeBar(sf::RenderTarget* target, Constants& constants, Player& player, Layout::Layout& layout);
}
