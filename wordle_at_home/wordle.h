#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <random>
#include "layout.h"
#include "constants.h"


enum LetterStates{
    DOES_NOT_EXIST,
    EXISTS,
    AT_RIGHT_POSITION
};

struct Wordle{
public:
	Wordle(Constants&);
	Wordle(Constants&, size_t wordlength);

	void receiveInput(const sf::Event&);

	void update();
	void render(sf::RenderTarget* target);
	
	bool getPlayerWon();
	
private:
	std::mt19937_64 randomState;

	std::wstring currentWord = L"";
	std::wstring currentUserInput;
	std::vector<std::wstring> userInputHistory;
	bool playerWon = false;
	size_t wordLength;
	
	Constants& constants;
	
	Layout::Layout layout;
	
	static constexpr int MAX_GUESSES = 7;
	
	std::vector<LetterStates> getLetterStates(std::wstring userInput);
	
	void makeLayout();
	
	void makeCharLayoutRow();
	std::wstring getRandomWord(int length);
};
