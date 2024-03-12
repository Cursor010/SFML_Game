#include "Player.h"

// Player
//------------------------------------------------------------------------------------------------------------
Player::Player(const sf::RenderWindow& window, const sf::Vector2f& pos, std::string const& texture, float time, const sf::Vector2i& spriteSize, const sf::Vector2i& framesOffset, const unsigned int frame)
    : Entity(window, pos, texture, time, spriteSize, framesOffset, frame),
    jump_height_(static_cast<float>((112 * window.getSize().y) / 720)), last_position_(0),
    scale_(sf::Vector2f(static_cast<float>((1.55 * window.getSize().x) / 1280), static_cast<float>((1.6 * window.getSize().y) / 720))),
    is_squat_(false), is_jump_(false), on_ground_(true), is_repulse_(false), Map_("../images/map1.png", sf::Vector2f(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)))
{
    window_size_.x = static_cast<float>(window.getSize().x);
    window_size_.y = static_cast<float>(window.getSize().y);

    sprite_object_.setScale(scale_);

    const unsigned int start_pos_y = framesOffset.y + spriteSize.y;

    auto& anim_walk = AnimObject_.createAnimation("Walk", texture, sf::seconds(time), true);
    auto& anim_stay = AnimObject_.createAnimation("Stay", texture, sf::seconds(time), true);
    auto& anim_jump_start = AnimObject_.createAnimation("JumpStart", texture, sf::seconds(time), true);
    auto& anim_jump_end = AnimObject_.createAnimation("JumpEnd", texture, sf::seconds(time), true);
    auto& anim_sit = AnimObject_.createAnimation("Sit", texture, sf::seconds(time), true);
    auto& anim_discard = AnimObject_.createAnimation("Discard", texture, sf::seconds(time), true);

    anim_walk.addFrames(sf::Vector2i(0, 3 * start_pos_y), spriteSize, framesOffset, frame, 1);
    anim_stay.addFrames(sf::Vector2i(0, start_pos_y), spriteSize, framesOffset, frame - 1, 1);
    anim_jump_start.addFrames(sf::Vector2i(framesOffset.x, 4 * start_pos_y), spriteSize, framesOffset, 1, 1);
    anim_jump_end.addFrames(sf::Vector2i(0, 6 * start_pos_y), spriteSize, framesOffset, 1, 1);
    anim_sit.addFrames(sf::Vector2i(0, 9 * start_pos_y + 16), sf::Vector2i(spriteSize.x, spriteSize.y - 16), framesOffset, frame - 1, 1);
    anim_discard.addFrames(sf::Vector2i(0, 17 * start_pos_y), sf::Vector2i(spriteSize.x + 20, spriteSize.y), sf::Vector2i(framesOffset.x - 5, framesOffset.y), frame - 3, 1);
}
//------------------------------------------------------------------------------------------------------------
void Player::update(sf::Time const& deltaTime)
{
    if (dead_)
    {
        return;
    }

    AnimObject_.update(deltaTime);

    Map_.checkInteractions(sprite_object_, on_ground_, &is_jump_);

    time_update_ += deltaTime;

    if (time_update_ > sf::milliseconds(3))
    {
        updateStates();
        updatePosition();

        time_update_ = sf::microseconds(0);
    }
}
//------------------------------------------------------------------------------------------------------------
void Player::updateStates()
{
    if (sprite_object_.getPosition().y + sprite_object_.getGlobalBounds().height >= Borders_.max_y)
    {
        on_ground_ = true;
    }

    if (is_jump_)
    {
        float jumpThreshold = is_repulse_ ? 0.8f : 1.0f;
        float jumpHeightCheck = jump_height_ * jumpThreshold;

        if (last_position_ - jumpHeightCheck > sprite_object_.getPosition().y)
        {
            is_jump_ = false;
        }
        else
        {
            setState(State::MOVE_UP, 1.5f);
        }
    }
    if (!on_ground_ && !is_jump_)
    {
        setState(State::MOVE_DOWN, 1.5f);
    }

    if (on_ground_)
    {
        if (is_repulse_)
        {
            is_repulse_ = false;
            stepx_ = 0;
        }

        if (stepx_ > 0)
        {
            setState(Player::State::MOVE_RIGHT, 0.5f);
        }
        else if (stepx_ < 0)
        {
            setState(Player::State::MOVE_LEFT, 0.5f);
        }
    }

    if (sprite_object_.getPosition().y <= Borders_.min_y)
    {
        is_jump_ = false;
    }
}
//------------------------------------------------------------------------------------------------------------
void Player::updatePosition()
{
    if (!dead_)
    {
        sf::Vector2f newPosition = sprite_object_.getPosition() + sf::Vector2f(stepx_, stepy_);

        newPosition.x = std::clamp(newPosition.x, Borders_.min_x + sprite_object_.getGlobalBounds().width, Borders_.max_x - sprite_object_.getGlobalBounds().width);
        newPosition.y = std::clamp(newPosition.y, Borders_.min_y, Borders_.max_y - sprite_object_.getGlobalBounds().height);

        sprite_object_.setPosition(newPosition);
    }
    else
    {
        sprite_object_.setPosition(Borders_.max_x / 2, Borders_.max_y - sprite_object_.getGlobalBounds().height);
        on_ground_ = true;
        is_jump_ = false;
        is_squat_ = false;
        dead_ = false;
        setState(State::STAY);
    }
}
//------------------------------------------------------------------------------------------------------------
void Player::toggleSpriteTransparency() {

    sf::Color color = sprite_object_.getColor();

    if (color.a < 255)
    {
        color.a = 255;
    }
    else
    {
        color.a = 150;
    }

    sprite_object_.setColor(color);
}
//------------------------------------------------------------------------------------------------------------
void Player::setState(State state, float step)
{
    if (dead_)
    {
        return;
    }

    float displacement = 0.0f;

    switch (state)
    {
    case State::MOVE_UP:
    {
        if (sprite_object_.getPosition().y > Borders_.min_y)
        {
            sprite_object_.move(0, (-step * window_size_.y) / 720);

            if (!is_repulse_)
            {
                if (AnimObject_.getCurrentAnimationName() != "JumpStart")
                {
                    AnimObject_.switchAnimation("JumpStart");
                }
            }
            else
            {
                if (AnimObject_.getCurrentAnimationName() != "Discard")
                {
                    AnimObject_.switchAnimation("Discard");
                }
            }
        }
        break;
    }
    case State::MOVE_DOWN:
    {
        if (sprite_object_.getPosition().y < Borders_.max_y)
        {
            sprite_object_.move(0, (step * window_size_.y) / 720);

            if (!is_repulse_)
            {
                if (AnimObject_.getCurrentAnimationName() != "JumpEnd")
                {
                    AnimObject_.switchAnimation("JumpEnd");
                }
            }
            else
            {
                if (AnimObject_.getCurrentAnimationName() != "Discard")
                {
                    AnimObject_.switchAnimation("Discard");
                }
            }
        }
        break;
    }
    case State::MOVE_RIGHT:
    {
        if (!is_squat_ && sprite_object_.getPosition().x < Borders_.max_x)
        {
            if (!on_ground_)
            {
                stepx_ = (step * 0.3f * window_size_.x) / 1280;
            }
            else
            {
                stepx_ = (step * window_size_.x) / 1280;
            }

            if (AnimObject_.getCurrentAnimationName() != "Walk")
            {
                AnimObject_.switchAnimation("Walk");
            }
        }

        sprite_object_.setScale(is_repulse_ ? sf::Vector2f(-scale_.x, scale_.y) : scale_);
        break;
    }
    case State::MOVE_LEFT:
    {
        if (!is_squat_ && sprite_object_.getPosition().x > Borders_.min_x)
        {
            if (!on_ground_)
            {
                stepx_ = (-step * 0.3f * window_size_.x) / 1280;
            }
            else
            {
                stepx_ = (-step * window_size_.x) / 1280;
            }
            if (AnimObject_.getCurrentAnimationName() != "Walk")
            {
                AnimObject_.switchAnimation("Walk");
            }
        }
        sprite_object_.setScale(is_repulse_ ? scale_ : sf::Vector2f(-scale_.x, scale_.y));
        break;
    }
    case State::STAY:
    {
        if (AnimObject_.getCurrentAnimationName() != "Stay")
        {
            AnimObject_.switchAnimation("Stay");
        }

        break;
    }
    case State::SQUAT:
    {
        stepx_ = 0;

        if (!is_squat_)
        {
            sprite_object_.setPosition(sprite_object_.getPosition().x, sprite_object_.getPosition().y + 16 * scale_.y);

        }

        if (AnimObject_.getCurrentAnimationName() != "Sit")
        {
            AnimObject_.switchAnimation("Sit");
        }

        break;
    }
    default:
        break;
    }

    sprite_object_.setPosition(sprite_object_.getPosition().x + (stepx_ * step) + displacement, sprite_object_.getPosition().y + (stepy_ * step));
}
//------------------------------------------------------------------------------------------------------------
void Player::setSquatState(const bool stsquat)
{
    is_squat_ = stsquat;
}
//------------------------------------------------------------------------------------------------------------
void Player::setJumpState(const bool stjump)
{
    is_jump_ = stjump;
}
//------------------------------------------------------------------------------------------------------------
void Player::setLastPosition(const float last_pos)
{
    last_position_ = last_pos;
}
//------------------------------------------------------------------------------------------------------------
void Player::setOnGroundState(bool stground)
{
    on_ground_ = stground;
}
//------------------------------------------------------------------------------------------------------------
void Player::setRepulseState(bool strepulse)
{
    is_repulse_ = strepulse;
}
//------------------------------------------------------------------------------------------------------------
const sf::Vector2f Player::getSteps() const
{
    return sf::Vector2f(stepx_, stepy_);
}
//------------------------------------------------------------------------------------------------------------
const bool Player::getOnGroundState() const
{
    return on_ground_;
}
//------------------------------------------------------------------------------------------------------------
const bool Player::getSquatState() const
{
    return is_squat_;
}
//------------------------------------------------------------------------------------------------------------
const bool Player::getRepulseState() const
{
    return is_repulse_;
}
//------------------------------------------------------------------------------------------------------------