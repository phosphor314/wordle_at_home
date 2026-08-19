#include "layout.h"
#include <SFML/Graphics.hpp>


sf::FloatRect Layout::Layout::getContainerBounds(size_t containerInd) const{
    const Container& c = containers[containerInd];
    
    if (c.parent == std::numeric_limits<size_t>::max()){
        return bounds;
    }
    
    const Container& parent = containers[c.parent];
    
    float divParamSum = 0;
    int nanCount = 0;
    for (int i=0; i < parent.children.size(); ++i){
        if (!std::isnan(containers[parent.children[i]].divisionParam)){
            divParamSum += containers[parent.children[i]].divisionParam;
        }
        else{
            ++nanCount;
        }
    }
    
    assert(divParamSum <= 1.0f);
    
    float nearEdge = 0.0f;
    for (int i=0; i < c.parentChildInd; ++i){
        if (!std::isnan(containers[parent.children[i]].divisionParam)){
            nearEdge += containers[parent.children[i]].divisionParam;
        }
        else{
            nearEdge += (1.0f-divParamSum)/nanCount;
        }
    }
    
    float shortDim;
    if (isnan(c.divisionParam)) {shortDim = (1.0f - divParamSum) / nanCount;}
    else {shortDim = c.divisionParam;}
    sf::FloatRect relBounds;
    
    switch (parent.divisionType){
        case LATERAL:
			relBounds = sf::FloatRect(sf::Vector2f(nearEdge, 0.0f), sf::Vector2f(shortDim, 1.0f));
			break;
		case VERTICAL:
			relBounds = sf::FloatRect(sf::Vector2f(0.0f, nearEdge), sf::Vector2f(1.0f, shortDim));
			break;
		case NONE:
			assert(parent.children.size() == 1);
			relBounds = sf::FloatRect(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(1.0f, 1.0f));
			break;
		default:
			throw std::runtime_error("Unexpected division type!");
    }
    
    sf::FloatRect parentBounds = getContainerBounds(c.parent);
    
    relBounds.position = relBounds.position.componentWiseMul(parentBounds.size) + parentBounds.position;
    relBounds.size = relBounds.size.componentWiseMul(parentBounds.size);
    relBounds.position.x += c.padding;
    relBounds.position.y += c.padding;
    relBounds.size.x -= 2*c.padding;
    relBounds.size.y -= 2*c.padding;
    
    return relBounds;
}

sf::FloatRect Layout::Layout::getElementBounds(size_t elementInd, size_t containerInd) const{
    const Element* e = nullptr;
    size_t counter = 0;
    for (const Element& elem : elements){
        if (elem.containerInd == containerInd){
            if (counter == elementInd){
                e = &elem;
                break;
            }
            ++counter;
        }
    }
    
    assert(e);
    
    sf::FloatRect parentBounds = getContainerBounds(e->containerInd);
    
    return sf::FloatRect(e->pos*parentBounds.size.x + parentBounds.position, e->extent*parentBounds.size.x);
}

Layout::Element& Layout::Layout::getElement(size_t elementInd, size_t containerInd){
    Element* e = nullptr;
    size_t counter = 0;
    for (Element& elem : elements){
        if (elem.containerInd == containerInd){
            if (counter == elementInd){
                e = &elem;
                break;
            }
            ++counter;
        }
    }
    
    assert(e);
    
    return *e;
}

size_t Layout::Layout::getNextElementIdx(size_t container){
    size_t counter = 0;
    for (const Element& elem : elements){
        if (elem.containerInd == container){
            ++counter;
        }
    }
    
    return counter;
}

void Layout::Layout::subdivideContainer(size_t container, float divisionParam, DivisionType divType){
	containers[container].children.emplace_back(containers.size());
	if (divType != NONE){
		containers[container].divisionType = divType;
	}
	
	containers.emplace_back();
	containers.back().parentChildInd = containers[container].children.size() - 1;
	containers.back().parent = container;
	containers.back().divisionParam = divisionParam;
}

void Layout::Layout::addElement(Element e){
    elements.push_back(e);
}
