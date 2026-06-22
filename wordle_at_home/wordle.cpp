#include "wordle.h"
#include <fstream>
#include <iostream>


Wordle::Wordle()
{
	monospace = sf::Font("SpaceMono-Regular.ttf");
	
	std::random_device rd;
	randomState = std::mt19937_64(rd());
	
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
			zeichen = L"";
		}
		else {
			zeichen += c; 
		}
	}
	if (zeichen != L"") {
		words.push_back(zeichen);
		zeichen = L"";
	}
}

void Wordle::update(){
    if (currentWord == L""){
    	std::uniform_int_distribution<size_t> randInd(0, words.size());
    	currentWord = words[randInd(randomState)];
    	std::wcout << currentWord << L"\n";
    }
    
    if (currentUserInput.size() == currentWord.size()){
    	userInputHistory.push_back(currentUserInput);
    	std::vector<LetterStates> ls = getLetterStates(currentUserInput);
    	currentUserInput = L"";
    	bool done = true;
    	for (LetterStates l : ls){
    		if (l != AT_RIGHT_POSITION){
    			done = false;
    			break;
    		}
    	}
    	if (done){
    		// YAY
    		exit(0);
    	}
    	else if (userInputHistory.size() == MAX_GUESSES){
    		exit(1);
    	}
    }
}

std::vector<LetterStates> Wordle::getLetterStates(std::wstring userInput){
	std::vector<LetterStates> out;
	out.resize(currentWord.size());
	assert(userInput.size() == currentWord.size());
	for (int i=0; i < currentWord.size(); ++i){
	    if (userInput[i] == currentWord[i]){
	        out[i] = AT_RIGHT_POSITION;
	    }
	    else {
	   		for (int j=0; j < currentWord.size(); ++j){
	   		    if (currentWord[j] == userInput[i]){
	   		        out[i] = EXISTS;
	   		    }
	   		}
	    }
	}
	
	return out;
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
