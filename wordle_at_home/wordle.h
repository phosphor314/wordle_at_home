#pragma once

#include <vector>
#include <SFML/Graphics.hpp>


enum LetterStates{
    DOES_NOT_EXIST,
    EXISTS,
    AT_RIGHT_POSITION
};

struct Wordle{
public:
	Wordle();

	void update();
	void render(sf::RenderTarget* target);
	
private:
	std::vector<std::string> words;
	std::wstring currentWord;
	
	std::vector<LetterStates> getLetterStates(std::wstring userInput);
};
