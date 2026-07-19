#include "wordle.h"
#include <SFML/Graphics.hpp>


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
	
	Wordle game;
	
	window.setFramerateLimit(60);
	
	while (window.isOpen()) {
		windowEvent = window.pollEvent();
		while (windowEvent.has_value()) {
			if (windowEvent.value().is<sf::Event::Closed>()) {
				window.close();
			}
			else {
				game.receiveInput(windowEvent.value());
			}
			windowEvent = window.pollEvent();
		}

		window.clear();
		game.render(&window);		
		window.display();
		
		game.update();
	}
}
