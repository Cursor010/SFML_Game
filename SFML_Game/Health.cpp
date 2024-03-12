#include "Health.h"

// Health
//------------------------------------------------------------------------------------------------------------
Health::Health(const sf::RenderWindow& window, const sf::Vector2f& pos, std::string const& texture, float time, const sf::Vector2i& spriteSize, const sf::Vector2i& framesOffset, const unsigned int frame)
    : Entity(window, pos, texture, time, spriteSize, framesOffset, frame), amount_health_(3), is_animation_update_(true)
{
    sprite_object_.scale(static_cast<float>((window.getSize().x * 1.5) / 1280), static_cast<float>((window.getSize().y * 1.5) / 720));

    auto& anim_one_heart = AnimObject_.createAnimation("oneHeart", texture, sf::seconds(time), true);
    auto& anim_two_heart = AnimObject_.createAnimation("twoHeart", texture, sf::seconds(time), true);
    auto& anim_three_heart = AnimObject_.createAnimation("threeHeart", texture, sf::seconds(time), true);

    anim_one_heart.addFrames(sf::Vector2i(0, (framesOffset.y + spriteSize.y) * 2), spriteSize, framesOffset, frame, 1);
    anim_two_heart.addFrames(sf::Vector2i(0, framesOffset.y + spriteSize.y), spriteSize, framesOffset, frame, 1);
    anim_three_heart.addFrames(sf::Vector2i(0, 0), spriteSize, framesOffset, frame, 1);
}
//------------------------------------------------------------------------------------------------------------
void Health::update(sf::Time const& deltaTime)
{
    if (!dead_)
    {
        time_update_ += deltaTime;

        if (is_animation_update_)
        {
            std::string animationName;

            switch (amount_health_)
            {
            case 3:
                animationName = "threeHeart";
                break;

            case 2:
                animationName = "twoHeart";
                break;

            case 1:
                animationName = "oneHeart";
                break;

            default:
                animationName = "";
                break;
            }

            if (!animationName.empty() && AnimObject_.getCurrentAnimationName() != animationName)
            {
                AnimObject_.switchAnimation(animationName);
                AnimObject_.update(deltaTime);
                is_animation_update_ = false;
            }
        }
    }
}
//------------------------------------------------------------------------------------------------------------
void Health::setHealth(unsigned short health)
{
    if (health <= 3 && health > 0)
    {
        amount_health_ = health;
        is_animation_update_ = true;
    }
}
//------------------------------------------------------------------------------------------------------------
const unsigned short Health::getHealth() const
{
    return amount_health_;
}
//------------------------------------------------------------------------------------------------------------
