#pragma once

#include "Entity.h"

//------------------------------------------------------------------------------------------------------------
class Health : public Entity {
public:
    Health(const sf::RenderWindow& window, const sf::Vector2f& pos, std::string const& texture, float time, const sf::Vector2i& spriteSize, const sf::Vector2i& framesOffset, const unsigned int frame);

    void update(sf::Time const& deltaTime) override;

    void setHealth(unsigned short health = 3);

    const unsigned short getHealth() const;

private:
    unsigned short amount_health_;

    bool is_animation_update_;
};
//------------------------------------------------------------------------------------------------------------
