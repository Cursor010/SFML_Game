#pragma once

#include <vector>

#include "SFML/Graphics.hpp"
#include "AssetManager.h"

//------------------------------------------------------------------------------------------------------------
class Map {
public:
    Map(std::string filePath, const sf::Vector2f& windowSize);

    void createObstacle(std::string& filePath, float x, float y, float scaleX, float scaleY = 1.f);

    void checkInteractions(sf::Sprite& spriteObject, bool& onGround, bool* isJump = nullptr);

    void draw(sf::RenderWindow& window);

private:
    const sf::Vector2f window_size_;

    std::vector<sf::Sprite> obstacles_;

    short current_obstacle_;
};
//------------------------------------------------------------------------------------------------------------