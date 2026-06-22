#include "wordle.h"
#include <fstream>

std::vector<LetterStates> Wordle::getLetterStates(std::wstring userInput){
	for (wchar_t c : userInput){
	    
	}
}



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