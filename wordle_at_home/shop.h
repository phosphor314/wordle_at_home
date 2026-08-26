#include "constants.h"
#include "player.h"
#include <unordered_map>

struct Shop{
    Shop(Constants&, Player&);
    
    void receiveInput(const sf::Event&);

	void update();
	void render(sf::RenderTarget* target);
	
	bool getPlayerDone();
    
  private:
    Constants& constants;
    Player& player;
    
    bool playerDone;
    std::unordered_map<sf::Keyboard::Key, size_t> buttonMap;
    std::vector<std::pair<int, Upgrades>> offers;
};
