#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <limits>
#include <math.h>


namespace Layout{
    enum DivisionType{
		NONE,
        LATERAL,
        VERTICAL
    };
    
    struct Container{
    public:
    	size_t parent = std::numeric_limits<size_t>::max();
    	size_t parentChildInd;
    	std::vector<size_t> children;
		DivisionType divisionType = NONE;
		float divisionParam = NAN;
		float padding = 8.0f;
    };
    
    struct Element{
        size_t containerInd;
        sf::Vector2f pos; // relative to container
        sf::Vector2f extent; // relative to container
    };
    
    struct Layout{
    public:
    	sf::FloatRect bounds;
    	
    	sf::FloatRect getContainerBounds(size_t containerInd) const;
    	sf::FloatRect getElementBounds(size_t elementInd, size_t containerInd) const;
    	Element& getElement(size_t elementInd, size_t containerInd);
    	
    	void subdivideContainer(size_t container, float divisionParam = NAN, DivisionType divType = NONE);
    	void addElement(Element);
    	
    	size_t getNextElementIdx(size_t container);
    private:
        std::vector<Container> containers = {Container()};
    	std::vector<Element> elements;
    };
}
