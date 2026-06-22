#include "wordle.h"
#include <fstream>


Wordle::Wordle()
{
	std::basic_ifstream<wchar_t> german;
	german.open("german.txt", std::ios_base::ate);
	std::vector<wchar_t> content;
	content.resize(german.tellg());
	german.seekg(0);
	german.read(content.data(), content.size());

	std::wstring zeichen;
	for (wchar_t c : content) {
		if (c == '\n') { 
			words.push_back(zeichen);
			zeichen = '';
		}
		else {
			zeichen += c; 
		}
	}
	if (zeichen != '') {
		words.push_back(zeichen);
		zeichen = '';
	}
}

void Wordle::update(){
    if (currentWord == L""){
    	std::uniform_int_distribution<size_t> randInd(0, words.size());
    	currentWord = words[randInd(randomState)];
    }
    
    if (currentUserInput.size() == currentWord.size()){
    	userInputHistory.push_back(currentUserInput);
    	if (currentUserInput == currentWord){
    		// YAY
    		exit(0);
    	}
    	else if (userInputHistory.size() == MAX_GUESSES){
    		exit(1);
    	}
    }
}

void Wordle::enterChar(wchar_t c){
	currentUserInput += c;
}

void Wordle::render(sf::RenderTarget* target){
	for (int i=0; i < userInputHistory.size(); ++i){
		std::wstring w = userInputHistory[i];
		std::vector<LetterStates> ls = getLetterStates(w);
		
		for (int j=0; j < w.size(); ++j){
			sf::Text C(monospace);
			C.setString(w[j]);
			C.setPosition(sf::Vector2f(20.0f*j, 20.0f*i));
			switch (ls[j]){
				case EXISTS: C.setFillColor(sf::Color::Yellow); break;
				case DOES_NOT_EXIST: C.setFillColor(sf::Color(128, 128, 128, 128)); break;;
				case AT_RIGHT_POSITION: C.setFillColor(sf::Color::Green); break;
			}
			target->draw(C);
		}
	}
	
	std::wstring w = currentUserInput;
	for (int j=0; j < w.size(); ++j){
		sf::Text C(monospace);
		C.setString(w[j]);
		C.setPosition(sf::Vector2f(20.0f*j, 20.0f*userInputHistory.size()));
		target->draw(C);
	}
}
