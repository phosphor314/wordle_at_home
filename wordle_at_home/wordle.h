#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include <random>
#include "layout.h"


enum LetterStates{
    DOES_NOT_EXIST,
    EXISTS,
    AT_RIGHT_POSITION
};

struct Wordle{
public:
	Wordle();

	void enterChar(wchar_t);

	void update();
	void render(sf::RenderTarget* target);
	
private:
	std::mt19937_64 randomState;

	std::vector<std::wstring> words;
	std::wstring currentWord = L"";
	std::wstring currentUserInput;
	std::vector<std::wstring> userInputHistory;
	
	sf::Font monospace;
	
	Layout::Layout layout;
	
	static constexpr int MAX_GUESSES = 7;
	
	std::vector<LetterStates> getLetterStates(std::wstring userInput);
	
	void makeLayout();
	void loadWords();
	
	void makeCharLayoutRow();
	std::wstring getRandomWord(int length);
};
