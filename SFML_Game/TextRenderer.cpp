#include "TextRenderer.h"

// TextRenderer
//------------------------------------------------------------------------------------------------------------
TextRenderer::TextRenderer(const std::string& fontPath, const unsigned int size, const sf::Vector2f& pos, const std::string& str, const sf::Color& color)
    : text_position_(pos), current_y_(pos.y), is_move_up_(true)
{
    text_.setFont(AssetManager::GetFont(fontPath));
    text_.setCharacterSize(size);
    text_.setPosition(text_position_);
    text_.setString(str);
    text_.setFillColor(color);
    text_.setStyle(sf::Text::Bold);
    text_.setScale(2.f, 1.3f);
}
//------------------------------------------------------------------------------------------------------------
void TextRenderer::draw(sf::RenderWindow& window) const
{
    window.draw(selection_);
    window.draw(text_);
}
//------------------------------------------------------------------------------------------------------------
void TextRenderer::setText(const std::string& str)
{
    text_.setString(str);
}
//------------------------------------------------------------------------------------------------------------
void TextRenderer::align(const float width, const float height)
{
    if (width != 0)
    {
        text_position_.x = (width - text_.getGlobalBounds().width) / 2;
    }
    if (height != 0)
    {
        text_position_.y = (height - text_.getGlobalBounds().height) / 2;
    }

    text_.setPosition(text_position_.x, text_position_.y);
}
//------------------------------------------------------------------------------------------------------------
void TextRenderer::setPosition(const sf::Vector2f& pos)
{
    text_position_ = pos;
    text_.setPosition(text_position_.x, text_position_.y);
}
//------------------------------------------------------------------------------------------------------------
void TextRenderer::setTextColor(const sf::Color& color)
{
    text_.setFillColor(color);
}
//------------------------------------------------------------------------------------------------------------
void TextRenderer::setSelection(const sf::Vector2f& indentation, const sf::Color& bgcolor, const short thickness, const sf::Color& bordercolor)
{
    selection_indentation_ = sf::Vector2f(indentation.x, indentation.y);
    selection_.setSize(sf::Vector2f(static_cast<float>(text_.getGlobalBounds().width + indentation.x), static_cast<float>(text_.getLocalBounds().height + indentation.y)));
    selection_.setFillColor(bgcolor);
    selection_.setOutlineThickness(thickness);
    selection_.setOutlineColor(bordercolor);
    selection_.setPosition(static_cast<float>(text_position_.x + text_.getLocalBounds().left - static_cast<float>(indentation.x) / 2), static_cast<float>(text_position_.y + text_.getLocalBounds().top * 1.3f - static_cast<float>(indentation.y) / 4));
}
//------------------------------------------------------------------------------------------------------------
void TextRenderer::setSelectionColor(const sf::Color& bgcolor, const sf::Color& bordercolor)
{
    selection_.setFillColor(bgcolor);
    selection_.setOutlineColor(bordercolor);
}
//------------------------------------------------------------------------------------------------------------
void TextRenderer::setLimitMovement(const float speed, const float startPos, const int range)
{
    if (is_move_up_)
    {
        current_y_ -= speed;
        if (current_y_ <= startPos - range) {
            is_move_up_ = false;
        }
    }
    else
    {
        current_y_ += speed;
        if (current_y_ >= startPos + range) {
            is_move_up_ = true;
        }
    }
    text_position_.y = current_y_;

    text_.setPosition(text_position_);
}
//------------------------------------------------------------------------------------------------------------
const sf::Vector2f TextRenderer::getPosition() const
{
    return text_.getPosition();
}
//------------------------------------------------------------------------------------------------------------