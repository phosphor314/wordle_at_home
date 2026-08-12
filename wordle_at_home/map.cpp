#include "map.h"
#include <random>
#include <algorithm>


LevelMap::LevelMap(size_t length, Constants& c, Player& p) : constants(c), player(p), selectRectCentre(sf::Vector2f(), 0.5f){
    std::mt19937_64 rand;
    {
        std::random_device dev;
    	rand.seed(dev());
    }
    
    constexpr size_t MAX_WIDTH = 4;
    constexpr size_t MIN_WIDTH = 1;
    
    map.push_back({{NONE, {}, 0}});
    
    size_t counter = 1;
    for (size_t i=1; i < length; ++i){
        size_t layerWidth = std::uniform_int_distribution(MIN_WIDTH, MAX_WIDTH)(rand);
        map.push_back({});
        for (size_t j=0; j < layerWidth; ++j){
            map[i].push_back({WORDLE, {}, counter});
            ++counter;
        }
        std::vector<char> noParent(layerWidth);
        std::fill(noParent.begin(), noParent.end(), true);
        for (size_t j=0; j < map[i-1].size(); ++j){
            size_t child = std::uniform_int_distribution<size_t>(0, layerWidth-1)(rand);
            map[i-1][j].children.push_back(child);
            noParent[child] = false;
        }
        for (size_t j=0; j < layerWidth; ++j){
            if (noParent[j]){
                map[i-1][std::uniform_int_distribution<size_t>(0, map[i-1].size()-1)(rand)].children.push_back(j);
            }
        }
    }
    
    makeLayout();
}

void LevelMap::render(sf::RenderTarget* target){
    layout.bounds = sf::FloatRect(target->getViewport(target->getView()));
    renderMap(target);
    renderSelectedTileInfo(target);
}

void LevelMap::update(){
    if (
        start_level_input && 
        selectedLayer == currentLayer + 1 && 
        std::any_of(map[currentLayer][currentNode].children.begin(), map[currentLayer][currentNode].children.end(), [this](const size_t& x){return x==selectedNode;})){
			start_level = true;
			currentNode = selectedNode;
			currentLayer = selectedLayer;
    }
    start_level_input = false;
}

void LevelMap::receiveInput(const sf::Event& event){
    if (event.is<sf::Event::KeyPressed>()){
        const sf::Event::KeyPressed& keyEv = *event.getIf<sf::Event::KeyPressed>();
        if (keyEv.code == sf::Keyboard::Key::Up){
            --selectedNode;
        }
        else if (keyEv.code == sf::Keyboard::Key::Down){
            ++selectedNode;
        }
        else if (keyEv.code == sf::Keyboard::Key::Right){
            ++selectedLayer;
        }
        else if (keyEv.code == sf::Keyboard::Key::Left){
            --selectedLayer;
        }
        else if (keyEv.code == sf::Keyboard::Key::Enter){
            start_level_input = true;
        }
        
        selectedLayer = std::clamp(selectedLayer, 0, (int)map.size()-1);
        selectedNode = std::clamp(selectedNode, 0, (int)map[selectedLayer].size()-1);
    }
}

bool LevelMap::getSelectedLevel(Wordle& level){
    if (!start_level){return false;}
    new (&level) Wordle(constants, player, 7-currentLayer);
    start_level = false;
    start_level_input = false;
    return true;
}

void LevelMap::makeLayout(){
    constexpr float TILE_SIZE = 0.8/7;
    constexpr float TILE_PADDING = 0.2/7;
    constexpr float LAYER_SEPARATION = 0.4/7;
    
    layout.subdivideContainer(0, 0.75f, Layout::VERTICAL);
    layout.subdivideContainer(0);
    
    for (size_t i=0; i < map.size(); ++i){
        sf::Vector2f offset((1.0f-map.size()*(LAYER_SEPARATION+TILE_SIZE))*0.5f, (TILE_PADDING+1.0f-map[i].size()*(TILE_SIZE+TILE_PADDING))*0.5f);
        for (size_t j=0; j < map[i].size(); ++j){
            layout.addElement({
                MAP_LAYOUT_CONTAINER,
                sf::Vector2f((LAYER_SEPARATION+TILE_SIZE)*i, j*(TILE_SIZE+TILE_PADDING)) + offset,
                sf::Vector2f(TILE_SIZE, TILE_SIZE)
            });
        }
    }
}

void LevelMap::renderMap(sf::RenderTarget* target){
    sf::VertexArray vArray;
    vArray.setPrimitiveType(sf::PrimitiveType::Lines);
    
    for (size_t l=0; l < map.size(); ++l){
        for (size_t n=0; n < map[l].size(); ++n){
            sf::RectangleShape rect;
            sf::FloatRect elemBounds = layout.getElementBounds(map[l][n].elemIdx, MAP_LAYOUT_CONTAINER);
            rect.setSize(elemBounds.size);
            rect.setPosition(elemBounds.position);
            rect.setFillColor(sf::Color::Transparent);
            rect.setOutlineColor(sf::Color::White);
            rect.setOutlineThickness(1.0f);
            if (l == selectedLayer && n == selectedNode){
                selectRectCentre.setTarget(elemBounds.getCenter());
                if ((sf::Vector2f)selectRectCentre == sf::Vector2f()){
                    selectRectCentre.forceSet(selectRectCentre);
                }
            }
            if (l == currentLayer && n == currentNode){
                rect.setOutlineThickness(8.0f);
            }
            
            switch (map[l][n].type){
                case WORDLE:
                {
                	sf::Text text(constants.MONOSPACE_FONT);
                	text.setString("W");
                	text.setCharacterSize(elemBounds.size.y*text.getCharacterSize() / text.getLocalBounds().size.y*0.8f);
                	text.setOrigin(text.getLocalBounds().getCenter());
                	text.setPosition(elemBounds.getCenter());
                	text.setFillColor(sf::Color(128, 128, 128, 128));
                	target->draw(text);
                } 
                	break;
                default: break;
            }
            
            target->draw(rect);
            
            for (size_t ind : map[l][n].children){
                sf::Vertex vert;
                vert.color = sf::Color::White;
                vert.position = elemBounds.getCenter() + 0.5f*sf::Vector2f(elemBounds.size.x, 0.0f);
                vArray.append(vert);
                sf::FloatRect nElemBounds = layout.getElementBounds(map[l+1][ind].elemIdx, MAP_LAYOUT_CONTAINER);
                vert.position = nElemBounds.getCenter() - 0.5f*sf::Vector2f(nElemBounds.size.x, 0.0f);
                vArray.append(vert);
            }
        }
    }
    
    sf::RectangleShape selectRect;
    selectRect.setSize(layout.getElementBounds(0, MAP_LAYOUT_CONTAINER).size * 1.2f);
    selectRect.setOrigin(selectRect.getSize()*0.5f);
    selectRect.setPosition(selectRectCentre);
    selectRect.setFillColor(sf::Color::Transparent);
    selectRect.setOutlineColor(sf::Color::Yellow);
    selectRect.setOutlineThickness(1.0f);
    target->draw(selectRect);
    
    target->draw(vArray);
}

void LevelMap::renderSelectedTileInfo(sf::RenderTarget* target){
    sf::Text text(constants.MONOSPACE_FONT);
    switch (map[selectedLayer][selectedNode].type){
        case NONE:
			text.setString("Hier ist nichts");
			break;
		case WORDLE:
			text.setString("Wordle, nichts spezielles");
    		break;
    }
    
    sf::FloatRect layoutBounds = layout.getContainerBounds(NODE_INFO_CONTAINER);
    int charWidth = text.getCharacterSize();
    int currentLineWidth = 0;
    int lineWidthSinceLastSpace = 0;
    char* lastSpace = nullptr;
    std::string str = text.getString();
    for (char& c : str){
        if (c == ' '){
            lastSpace = &c;
        		lineWidthSinceLastSpace = 0;
        }
        if (currentLineWidth > layoutBounds.size.x){
            if (lastSpace){
                *lastSpace = '\n';
            		currentLineWidth = lineWidthSinceLastSpace;
            }
        }
        currentLineWidth += charWidth;
        lineWidthSinceLastSpace += charWidth;
    }
    
    text.setString(str);
    
    target->draw(text);
}
