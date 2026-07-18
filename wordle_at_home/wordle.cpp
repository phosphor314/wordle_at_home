#include "wordle.h"
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <algorithm>


Wordle::Wordle()
{
	monospace = sf::Font("SpaceMono-Regular.ttf");
	
	std::random_device rd;
	randomState = std::mt19937_64(rd());
	
	loadWords();
	
	makeLayout();
}

void Wordle::loadWords(){
	std::wifstream german;
	german.imbue(std::locale(""));
	german.open("german.txt", std::ios_base::ate);
	std::vector<wchar_t> content;
	content.resize(german.tellg());
	german.seekg(0);
	german.read(content.data(), content.size());
	
	std::wstring zeichen;
	for (wchar_t c : content) {
		if (c == L'\n') { 
			words.push_back(zeichen);
			zeichen = L"";
		}
		else if (c != L'\r') {
			zeichen += c; 
		}
	}
	if (zeichen != L"") {
		words.push_back(zeichen);
	}
	
	std::sort(words.begin(), words.end(), [](const std::wstring& a, const std::wstring& b){
		return a.length() < b.length();
	});
}

void Wordle::update(){
    if (currentWord == L""){
    	currentWord = getRandomWord(7);
    	std::wcout << currentWord << L"\n";
    	
    	makeCharLayoutRow();
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
    	
    	makeCharLayoutRow();
    }
}

std::wstring Wordle::getRandomWord(int length){
	int minInd = 0;
	int maxInd = words.size()-1;
	
	{
		int _minInd = 0;
		int _maxInd = words.size()-1;
		while (true){
			if (words[minInd].length() < length){
				_minInd = minInd;
				minInd = (minInd + _maxInd)/2+1;
				continue;
			}
			if (minInd > 0){
				if (words[minInd-1].length() >= length){
					if (words[minInd-1].length() > length){
						maxInd = minInd-1;
					}
					_maxInd = minInd;
					minInd = (minInd+_minInd)/2;
					continue;
				}
			}
			break;
		}
		minInd = minInd;
		_maxInd = maxInd;
		while (true){
			if (words[maxInd].length() > length){
				_maxInd = maxInd;
				maxInd = (maxInd + _minInd)/2;
				continue;
			}
			if (maxInd < words.size()-1){
				if (words[maxInd+1].length() == length){
					_minInd = maxInd;
					maxInd = (maxInd+_maxInd)/2+1;
					continue;
				}
			}
			break;
		}
	}
	
	std::uniform_int_distribution<int> dist(minInd, maxInd);
	return words[dist(randomState)];
}

std::vector<LetterStates> Wordle::getLetterStates(std::wstring userInput){
	std::vector<LetterStates> out;
	out.resize(currentWord.size());
	assert(userInput.size() == currentWord.size());
	
	std::unordered_map<wchar_t, int> free_letters;
	for (int i=0; i < currentWord.size(); ++i){
	    	if (!free_letters.count(currentWord[i])){
	    		free_letters[currentWord[i]] = 0;
	    	}
	    if (std::towupper(userInput[i]) == std::towupper(currentWord[i])){
	        out[i] = AT_RIGHT_POSITION;
	    }
	    else{
	    		++free_letters[std::towupper(currentWord[i])];
	    }
	}
	
	for (int i=0; i < currentWord.size(); ++i){
		if (out[i] == AT_RIGHT_POSITION){continue;}
		if (free_letters[std::towupper(userInput[i])]){
			--free_letters[std::towupper(userInput[i])];
			out[i] = EXISTS;
		}
	}
	
	return out;
}

void Wordle::enterChar(wchar_t c){
	currentUserInput += std::towupper(c);
}

void Wordle::render(sf::RenderTarget* target){
	layout.bounds = sf::FloatRect(target->getViewport(target->getView()));
	
	for (int i=0; i < userInputHistory.size(); ++i){
		std::wstring w = userInputHistory[i];
		std::vector<LetterStates> ls = getLetterStates(w);
		
		for (int j=0; j < w.size(); ++j){
			sf::FloatRect tileBounds = layout.getElementBounds(i*currentWord.size()+j, 2);
			
			sf::Text C(monospace);
			C.setString(w[j]);
			C.setPosition(tileBounds.getCenter()-C.getLocalBounds().getCenter());
			sf::Color fillColor;
			switch (ls[j]){
				case EXISTS: fillColor = sf::Color::Yellow; break;
				case DOES_NOT_EXIST: fillColor = sf::Color(128, 128, 128, 128); break;
				case AT_RIGHT_POSITION: fillColor = sf::Color::Green; break;
			}
			
			C.setFillColor(fillColor);
			
			target->draw(C);
		}
	}
	
	std::wstring& w = currentUserInput;
	for (int j=0; j < w.size(); ++j){
		sf::FloatRect tileBounds = layout.getElementBounds(userInputHistory.size()*currentWord.size()+j, 2);
		
		sf::Text C(monospace);
		C.setString(w[j]);
		C.setPosition(tileBounds.getCenter()-C.getLocalBounds().getCenter());
		target->draw(C);
	}
	
	for (int i=0; i < (userInputHistory.size()+1)*currentWord.size(); ++i){
		sf::RectangleShape rect;
		sf::FloatRect elemBounds = layout.getElementBounds(i, 2);
		rect.setSize(elemBounds.size);
		rect.setPosition(elemBounds.position);
		rect.setFillColor(sf::Color::Transparent);
		rect.setOutlineColor(sf::Color::White);
		rect.setOutlineThickness(1.0f);
		target->draw(rect);
	}
}

void Wordle::makeLayout(){
	layout.subdivideContainer(0, NAN, Layout::LATERAL);
	layout.subdivideContainer(0, 0.8f);
}

void Wordle::makeCharLayoutRow(){
	float WIDTH_PER_CHAR = 1.0f/currentWord.size();
	float CHAR_TILE_WIDTH = 0.8*WIDTH_PER_CHAR;
	float CHAR_TILE_PADDING = WIDTH_PER_CHAR - CHAR_TILE_WIDTH;
	
	for (int i=0; i < currentWord.size(); ++i){
		layout.addElement(Layout::Element{
			2,
			sf::Vector2f(WIDTH_PER_CHAR*i+CHAR_TILE_PADDING, WIDTH_PER_CHAR*userInputHistory.size()+CHAR_TILE_PADDING),
			sf::Vector2f(CHAR_TILE_WIDTH, CHAR_TILE_WIDTH)
		});
	}
}
