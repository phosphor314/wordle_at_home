#include "shop.h"
#include "player.h"


Shop::Shop(Constants& c, Player& p) : constants(c), player(p) {}
    
void Shop::receiveInput(const sf::Event& event){
    const sf::Event::KeyPressed* keyEv = event.getIf<sf::Event::KeyPressed>();
    if (keyEv){
        if (buttonMap.count(keyEv->code)){
            if (player.money >= offers[buttonMap[keyEv->code]].second){
                player.money -= offers[buttonMap[keyEv->code]].second;
            		player.upgrades.insert(offers[buttonMap[keyEv->code]].second);
            		offers.erase(offers.begin() + buttonMap[keyEv->code]);
            }
        }
    }
}

void Shop::update(){
    
}

void Shop::render(sf::RenderTarget* target){
    std::string text;
    for (const auto& x : offers){
        text.append("Purchase " + GET_UPGRADE_NAME) 
    }
}

bool Shop::getPlayerDone(){
    
}
