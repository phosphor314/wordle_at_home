#pragma once

#include <SFML/Graphics.hpp>
#include <vector>


namespace Layout{
    enum AlignmentBits{
        TOP = 0x1,
        BOTTOM = 0x2,
        LEFT = 0x4,
        RIGHT = 0x8
    };
    
    struct Container{
    public:
    	Container* parent = nullptr;
		AlignmentBits alignment;
		std::vector<Container> children;
		float padding = 0.0f;
		
		sf::FloatRect getAreaFactor(uint subcontainerInd);
		
		
    };
    
    
}
