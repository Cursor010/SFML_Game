#pragma once

#include <iostream>

#include "AssetManager.h"

//------------------------------------------------------------------------------------------------------------
class TextRenderer {
public:
    TextRenderer(const std::string& fontPath, const unsigned int size, const sf::Vector2f& pos, const std::string& str, const sf::Color& color);

    void draw(sf::RenderWindow& window) const;
    void align(const float width, const float height);

    void setText(const std::string& str);
    void setPosition(const sf::Vector2f& pos);
    void setTextColor(const sf::Color& color);
    void setSelection(const sf::Vector2f& indentation, const sf::Color& bgcolor, const short thickness, const sf::Color& bordercolor);
    void setSelectionColor(const sf::Color& bgcolor, const sf::Color& bordercolor);
    void setLimitMovement(const float speed, const float startPos, const int range);

    const sf::Vector2f getPosition() const;

private:
    sf::RectangleShape selection_;

    sf::Vector2f text_position_;
    sf::Vector2f selection_indentation_;

    sf::Text text_;

    bool is_move_up_;

    float current_y_;
};
//------------------------------------------------------------------------------------------------------------