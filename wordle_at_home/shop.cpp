#include "shop.h"
#include "SFML/Graphics/Text.hpp"
#include "player.h"
#include <sstream>

Shop::Shop(Constants &c, Player &p) : constants(c), player(p) {}

void Shop::receiveInput(const sf::Event &event) {
  const sf::Event::KeyPressed *keyEv = event.getIf<sf::Event::KeyPressed>();
  if (keyEv) {
    if (buttonMap.count(keyEv->code)) {
      if (player.money >= offers[buttonMap[keyEv->code]].second) {
        player.money -= offers[buttonMap[keyEv->code]].second;
        player.upgrades.insert(offers[buttonMap[keyEv->code]].second);
        offers.erase(offers.begin() + buttonMap[keyEv->code]);
      }
    } else if (keyEv->code == sf::Keyboard::Key::Escape) {
      playerDone = true;
    }
  }
}

void Shop::update() {}

void Shop::render(sf::RenderTarget *target) {
  std::stringstream ss;
  for (const auto &x : offers) {
    ss << "Purchase " << GET_UPGRADE_NAME(x.second) << " for " << x.first
       << " gold!\n";
  }

  sf::Text text(constants.SCHNORKEL_FONT);
  text.setString(ss.str());
  text.setPosition(sf::Vector2f(0.0f, 0.0f));
  target->draw(text);
}

bool Shop::getPlayerDone() { return true; } // return playerDone
