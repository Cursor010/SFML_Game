#include "Explosion.h"

// Explosion
//------------------------------------------------------------------------------------------------------------
Explosion::Explosion(const sf::RenderWindow& window, const sf::Vector2f& pos, std::string const& texture, float time, const sf::Vector2i& spriteSize, const sf::Vector2i& framesOffset, const unsigned int frame)
    : Entity(window, pos, texture, time, spriteSize, framesOffset, frame), max_frame_(frame - 2)
{
    sprite_object_.setScale((0.3f * window.getSize().x) / 1280, (0.3f * window.getSize().y) / 720);

    auto& animexplosion = AnimObject_.createAnimation("Explos", texture, sf::seconds(time), true);
    animexplosion.addFrames(sf::Vector2i(0, framesOffset.y), spriteSize, framesOffset, frame, 1);
}
//------------------------------------------------------------------------------------------------------------
void Explosion::update(sf::Time const& deltaTime)
{
    if (AnimObject_.getCurrentFrame() <= max_frame_)
    {
        AnimObject_.update(deltaTime);
    }
    else
    {
        dead_ = true;
    }

    time_update_ += deltaTime;
}
//------------------------------------------------------------------------------------------------------------