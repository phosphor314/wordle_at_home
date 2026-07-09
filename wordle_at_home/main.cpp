#include "wordle.h"
#include <SFML/Graphics.hpp>


#ifdef _WIN32
#include <direct.h>
#endif

int main(){
#ifdef _WIN32
	_chdir("../../..");
#endif
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
			if (windowEvent.value().is<sf::Event::TextEntered>()){
				const sf::Event::TextEntered* keyEv = windowEvent.value().getIf<sf::Event::TextEntered>();
				game.enterChar(keyEv->unicode);
			}
			windowEvent = window.pollEvent();
		}

		window.clear();
		game.render(&window);		
		window.display();
		
		game.update();
	}
}
