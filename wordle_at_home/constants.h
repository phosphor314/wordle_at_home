#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

struct Constants{
    Constants();
    sf::Font MONOSPACE_FONT = sf::Font("SpaceMono-Regular.ttf");
	std::vector<std::wstring> words;
};
