#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include <random>


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
	std::wstring currentWord;
	std::wstring currentUserInput;
	std::vector<std::wstring> userInputHistory;
	
	sf::Font monospace;
	
	static constexpr int MAX_GUESSES = 6;
	
	std::vector<LetterStates> getLetterStates(std::wstring userInput);
};
