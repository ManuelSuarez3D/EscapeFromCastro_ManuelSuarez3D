#pragma once


#include "Entity.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <vector>
#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include <algorithm>


namespace Physics
{
	sf::Vector2f getOverlapEntity(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b, std::string entityName = "default", std::string state = "fony");
	sf::Vector2f getOverlapMouse(sf::Vector2f a, std::shared_ptr<Entity> b);
	sf::Vector2f getPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);
};

