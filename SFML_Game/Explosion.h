#pragma once

#include "Entity.h"

//------------------------------------------------------------------------------------------------------------
class Explosion : public Entity
{
public:
    Explosion(const sf::RenderWindow& window, const sf::Vector2f& pos, std::string const& texture, float time, const sf::Vector2i& spriteSize, const sf::Vector2i& framesOffset, const unsigned int frame);

    void update(sf::Time const& deltaTime) override;

private:
    const unsigned short max_frame_;
};
//------------------------------------------------------------------------------------------------------------

