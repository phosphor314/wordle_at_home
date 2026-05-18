#include "wordle.h"
#include <SFML/Graphics.hpp>


int main(){
	sf::VideoMode videoMode = sf::VideoMode(sf::Vector2u{800, 800});
	sf::RenderWindow window = sf::RenderWindow(videoMode, "wordle at home");
	std::optional<sf::Event> windowEvent;
	window.setFramerateLimit(60);

	while (window.isOpen()) {
		windowEvent = window.pollEvent();
		while (windowEvent.has_value()) {
			if (windowEvent.value().is<sf::Event::Closed>()) {
				window.close();
			}
			windowEvent = window.pollEvent();
		}

		window.clear();
		window.display();
	}
}