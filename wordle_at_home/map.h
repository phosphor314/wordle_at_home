#pragma once

#include "constants.h"
#include "interpolate.hpp"
#include "layout.h"
#include "player.h"
#include "shop.h"
#include "wordle.h"
#include <SFML/Graphics.hpp>

enum class LevelType {
  WORDLE,
  SHOP,
};

struct LevelInfo {
  LevelInfo();
  ~LevelInfo();

  LevelType type;
  union {
    Wordle wordle;
    Shop shop;
  };
};

struct LevelMap {
public:
  LevelMap(size_t length, Constants &, Player &);

  void render(sf::RenderTarget *);
  void update();

  void receiveInput(const sf::Event &);

  bool getSelectedLevel(LevelInfo &level);

  enum class LocationType {
    NONE,
    WORDLE,
  };

  enum class EdgeType { NONE, SHOP };

  struct Edge {
    EdgeType type;
    size_t target;
    size_t elemIdx;
  };

  struct Node {
    LocationType type;
    std::vector<Edge> children;
    size_t elemIdx;
  };

private:
  static constexpr size_t MAP_LAYOUT_CONTAINER = 1;
  static constexpr size_t NODE_INFO_CONTAINER = 2;

  Layout::Layout layout;
  std::vector<std::vector<Node>> map;
  int selectedNode = 0;
  int selectedLayer = 0;
  int currentNode = 0;
  int currentLayer = 0;
  bool start_level_input = false;
  bool start_level = false;
  bool invokeShop = false;
  Interpolated<sf::Vector2f, easingFuns::EaseOutExpo> selectRectCentre;

  Constants &constants;
  Player &player;

  void makeLayout();

  void renderMap(sf::RenderTarget *);
  void renderSelectedTileInfo(sf::RenderTarget *);
};
