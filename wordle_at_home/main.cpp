#include "map.h"
#include <SFML/Graphics.hpp>
#include "constants.h"


#ifdef _WIN32
#include <direct.h>
#endif

int main(){
#ifdef _WIN32
	_chdir("../../..");
#endif
	std::setlocale(LC_ALL, "de_CH.iso88591");

	sf::VideoMode videoMode = sf::VideoMode(sf::Vector2u{800, 800});
	sf::RenderWindow window = sf::RenderWindow(videoMode, "wordle at home");
	std::optional<sf::Event> windowEvent;
	
	Constants constants;
	Player player;

	LevelMap levelMap(5, constants, player);
	Wordle wordle(constants, player);
	bool inLevelMap = true;
	
	window.setFramerateLimit(60);
	
	while (window.isOpen()) {
		windowEvent = window.pollEvent();
		while (windowEvent.has_value()) {
			if (windowEvent.value().is<sf::Event::Closed>()) {
				window.close();
			}
			else {
				if (inLevelMap){
					levelMap.receiveInput(windowEvent.value());
				}
				else{
					wordle.receiveInput(windowEvent.value());
				}
			}
			windowEvent = window.pollEvent();
		}

		window.clear();
		if (inLevelMap){
			levelMap.render(&window);
		}
		else{
			wordle.render(&window);
		}
		window.display();
		
		if (inLevelMap){
			levelMap.update();
			if (levelMap.getSelectedLevel(wordle)){
				inLevelMap = false;
			}
		}
		else{
			wordle.update();
			inLevelMap = wordle.getPlayerWon();
		}
	}
}
