#pragma once

#include "layout.h"
#include "constants.h"
#include "player.h"
#include <SFML/Graphics.hpp>
#include "interpolate.hpp"
#include "wordle.h"


struct LevelMap{
public:
	LevelMap(size_t length, Constants&, Player&);
	
	void render(sf::RenderTarget*);
	void update();
	
	void receiveInput(const sf::Event&);
	
	bool getSelectedLevel(Wordle& level);
	
	enum LocationType{
	    WORDLE,
	    NONE
	};
	
	struct Node{
	    LocationType type;
	    std::vector<size_t> children;
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
	Interpolated<sf::Vector2f, easingFuns::EaseOutExpo> selectRectCentre;
	
	Constants& constants;
	Player& player;
	
	void makeLayout();
	
	void renderMap(sf::RenderTarget*);
	void renderSelectedTileInfo(sf::RenderTarget*);
};
