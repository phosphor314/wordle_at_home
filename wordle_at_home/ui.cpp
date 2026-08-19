#include "ui.h"

#include <string>
#include <array>

void UI::renderUpgradeBar(sf::RenderTarget* target, Constants& constants, Player& player, Layout::Layout& layout){
	// Use root container (0) bounds to position bar
	sf::FloatRect bounds = layout.getContainerBounds(0);

	float barHeight = std::floor(bounds.size.y * 0.08f);
	sf::FloatRect barRect(sf::Vector2f(bounds.position.x + 8.0f, bounds.position.y + 8.0f), sf::Vector2f(bounds.size.x - 16.0f, barHeight - 8.0f));

	// Background
	sf::RectangleShape bg(sf::Vector2f(barRect.size.x, barRect.size.y));
	bg.setPosition(barRect.position);
	bg.setFillColor(sf::Color(32, 32, 32, 200));
	bg.setOutlineColor(sf::Color(255,255,255,100));
	bg.setOutlineThickness(1.0f);
	target->draw(bg);

	// Upgrade labels
	std::array<std::wstring,5> labels = {L"+1 Versuch", L"Endgeld", L"X-Wort", L"Geldx", L"Unverwundb."};

	size_t upgradeCount = labels.size();
	float padding = 6.0f;
	float slotW = (barRect.size.x - (upgradeCount+1)*padding) / (float)upgradeCount;
	float slotH = barRect.size.y - 2*padding;

	for (size_t i=0; i < upgradeCount; ++i){
		float x = barRect.position.x + padding + i*(slotW + padding);
		float y = barRect.position.y + padding;

		sf::RectangleShape slot(sf::Vector2f(slotW, slotH));
		slot.setPosition(sf::Vector2f(x, y));
		bool owned = false;
		switch (i){
			case 0: owned = player.upgrades.count(ADD_ATTEMPT); break;
			case 1: owned = player.upgrades.count(MONEY_END); break;
			case 2: owned = player.upgrades.count(X_WORD); break;
			case 3: owned = player.upgrades.count(MONEY_MULTIPLIER); break;
			case 4: owned = player.upgrades.count(INVINCIBILITY); break;
		}
		if (owned){
			slot.setFillColor(sf::Color(50, 150, 50, 220));
		}
		else{
			slot.setFillColor(sf::Color(80, 80, 80, 200));
		}
		slot.setOutlineColor(sf::Color::White);
		slot.setOutlineThickness(1.0f);
		target->draw(slot);

		// Text
		sf::Text text(constants.MONOSPACE_FONT);
		text.setString(labels[i]);
		// scale character size to slot height
		text.setCharacterSize((unsigned int)std::max(6.0f, slotH*0.45f));
		text.setFillColor(sf::Color::White);
		// center
		// center text in slot
		text.setOrigin(text.getLocalBounds().getCenter());
		text.setPosition(sf::Vector2f(x + slotW/2.0f, y + slotH/2.0f));
		target->draw(text);
	}
}
