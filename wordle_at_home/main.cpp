#include "constants.h"
#include "map.h"
#include <SFML/Graphics.hpp>

#ifdef _WIN32
#include <direct.h>
#endif

int main() {
#ifdef _WIN32
  _chdir("../../..");
#endif
  std::setlocale(LC_ALL, "de_CH.iso88591");
  Constants constants;
  Player player;

  LevelMap levelMap(5, constants, player);

  sf::VideoMode videoMode = sf::VideoMode(sf::Vector2u{800, 800});
  sf::RenderWindow window = sf::RenderWindow(videoMode, "wordle at home");
  std::optional<sf::Event> windowEvent;

  LevelInfo level;
  bool inLevelMap = true;

  window.setFramerateLimit(60);

  while (window.isOpen()) {
    windowEvent = window.pollEvent();
    while (windowEvent.has_value()) {
      if (windowEvent.value().is<sf::Event::Closed>()) {
        window.close();
      } else {
        if (inLevelMap) {
          levelMap.receiveInput(windowEvent.value());
        } else {
          switch (level.type) {
          case LevelType::WORDLE:
            level.wordle.receiveInput(windowEvent.value());
            break;
          case LevelType::SHOP:
            level.shop.receiveInput(windowEvent.value());
            break;
          }
        }
      }
      windowEvent = window.pollEvent();
    }

    window.clear();
    if (inLevelMap) {
      levelMap.render(&window);
    } else {
      switch (level.type) {
      case LevelType::WORDLE:
        level.wordle.render(&window);
        break;
      case LevelType::SHOP:
        level.shop.render(&window);
        break;
      }
    }
    window.display();

    if (inLevelMap) {
      levelMap.update();
      if (levelMap.getSelectedLevel(level)) {
        inLevelMap = false;
      }
    } else {
      switch (level.type) {
      case LevelType::WORDLE:
        level.wordle.update();
        inLevelMap = level.wordle.getPlayerWon();
        break;
      case LevelType::SHOP:
        level.shop.update();
        inLevelMap = level.shop.getPlayerDone();
        break;
      }
    }
  }
}
