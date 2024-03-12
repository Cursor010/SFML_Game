#include "Enemy.h"

// Enemy
//------------------------------------------------------------------------------------------------------------
Enemy::Enemy(const sf::RenderWindow& window, const sf::Vector2f& pos, std::string const& texture, float time, const sf::Vector2i& spriteSize, const sf::Vector2i& framesOffset, const unsigned int frame)
    : Entity(window, pos, texture, time, spriteSize, framesOffset, frame), right_movement_(true), scale_(sf::Vector2f((static_cast<float>(1.2 * window.getSize().x) / 1280), static_cast<float>((1.2 * window.getSize().y) / 720)))
{
    window_size_.x = static_cast<float>(window.getSize().x);
    window_size_.y = static_cast<float>(window.getSize().y);

    sprite_object_.scale(scale_);

    right_movement_ = (pos.x <= 0);

    auto& anim_walk = AnimObject_.createAnimation("Walk", texture, sf::seconds(time), true);
    anim_walk.addFrames(sf::Vector2i(0, 0), spriteSize, framesOffset, frame, 2);
}
//------------------------------------------------------------------------------------------------------------
void Enemy::update(sf::Time const& deltaTime)
{
    if (!dead_)
    {
        AnimObject_.update(deltaTime);
    }

    time_update_ += deltaTime;

    if (time_update_ > sf::milliseconds(3))
    {
        time_update_ = sf::microseconds(0);

        if (!dead_)
        {
            sprite_object_.move(stepx_, stepy_);
            updatePosition();
        }
    }
}
//------------------------------------------------------------------------------------------------------------
void Enemy::updatePosition()
{
    RandomNumberGenerator gen;

    float y_pos = gen.getRandom(static_cast<int>(window_size_.y / 720), static_cast<int>((window_size_.y * 6) / 720)) * 100;

    if (right_movement_)
    {
        if (sprite_object_.getPosition().x > Borders_.max_x - sprite_object_.getGlobalBounds().width)
        {
            right_movement_ = (gen.getRandom(0, 1) == 0);
            sprite_object_.setPosition(right_movement_ ? Borders_.min_x + sprite_object_.getGlobalBounds().width : Borders_.max_x - sprite_object_.getGlobalBounds().width, y_pos);
        }
    }
    else
    {
        if (sprite_object_.getPosition().x < Borders_.min_x)
        {
            right_movement_ = !(gen.getRandom(0, 1));
            sprite_object_.setPosition(right_movement_ ? Borders_.min_x + sprite_object_.getGlobalBounds().width : Borders_.max_x - sprite_object_.getGlobalBounds().width, y_pos);
        }
    }

    if (sprite_object_.getPosition().y < Borders_.min_y)
    {
        sprite_object_.setPosition(sprite_object_.getPosition().x, Borders_.min_y);
    }

    if (sprite_object_.getPosition().y > Borders_.max_y - sprite_object_.getGlobalBounds().height)
    {
        sprite_object_.setPosition(sprite_object_.getPosition().x, Borders_.max_y - sprite_object_.getGlobalBounds().height);
    }

    reflectSprite(right_movement_);
}
//------------------------------------------------------------------------------------------------------------
void Enemy::setDead(bool objectdead)
{
    sprite_object_.setPosition(right_movement_ ? window_size_.x : 0.0f, 0.0f);
}
//------------------------------------------------------------------------------------------------------------
void Enemy::setState(State state, float step)
{
    if (!dead_)
    {
        switch (state)
        {
        case State::MOVE_RIGHT:
        {
            if (sprite_object_.getPosition().x < Borders_.max_x)
            {
                stepx_ = (window_size_.x * step) / 1280;
            }
            break;
        }
        case State::MOVE_LEFT:
        {
            if (sprite_object_.getPosition().x > Borders_.min_x)
            {
                stepx_ = (window_size_.x * -step) / 1280;

            }
            break;
        }
        default:
            break;
        }
    }
}
//------------------------------------------------------------------------------------------------------------
const bool Enemy::getMovement() const
{
    return right_movement_;
}
//------------------------------------------------------------------------------------------------------------
std::unique_ptr<Enemy> Enemy::getEnemies(const sf::RenderWindow& window, const std::string& texture, float time, const sf::Vector2i& spriteSize, const sf::Vector2i& framesOffset, const unsigned int frame)
{
    RandomNumberGenerator gen;
    bool is_right = gen.getRandom(0, 1) == 0;

    float x_pos = is_right ? window.getSize().x + gen.getRandom(0, (window.getSize().x * 8) / 1280) * 125 : gen.getRandom(0, (window.getSize().x * 8) / 1280) * -125;
    float y_pos = gen.getRandom(window.getSize().y / 720, (window.getSize().y * 6) / 720) * 100;

    auto enemy = std::make_unique<Enemy>(window, sf::Vector2f(x_pos, y_pos), texture, time, spriteSize, framesOffset, frame);
    enemy->reflectSprite(is_right);

    return enemy;
}
//------------------------------------------------------------------------------------------------------------
bool Enemy::checkInteractions(Player& player, Health& health, sf::Sprite enemy, const sf::Time& deltaTime)
{
    bool is_interation = false;
    static sf::Clock damage_clock;
    static sf::Clock invulnerability_clock;
    static bool invulnerable = false;
    static const sf::Time invulnerability_duration = sf::milliseconds(1600);
    static const sf::Time blink_interval = sf::milliseconds(200);
    static short blink_count = 0;

    sf::FloatRect player_bounds = player.getSpriteObject().getGlobalBounds();
    bool player_is_dead = player.getDead();
    const unsigned short health_value = health.getHealth();

    sf::FloatRect enemyBounds = enemy.getGlobalBounds();
    bool enemyIntersectsBee = enemyBounds.intersects(player_bounds);

    if (enemyIntersectsBee && !player_is_dead)
    {
        if (!invulnerable)
        {
            invulnerable = true;
            invulnerability_clock.restart();
            damage_clock.restart();
            blink_count = 0;
            is_interation = true;
        }
    }

    if (invulnerable)
    {
        if (damage_clock.getElapsedTime() >= blink_interval && blink_count < 5)
        {
            player.toggleSpriteTransparency();
            blink_count++;
            damage_clock.restart();
        }
        if (invulnerability_clock.getElapsedTime() >= invulnerability_duration)
        {
            if (player.getSpriteObject().getColor().a != 255)
            {
                player.toggleSpriteTransparency();
            }
            invulnerable = false;
            blink_count = 0;
        }
    }

    return is_interation;
}
//------------------------------------------------------------------------------------------------------------
void Enemy::reflectSprite(bool rightMovment)
{
    if (rightMovment)
    {
        sprite_object_.setScale(-scale_.x, scale_.y);
    }
    else
    {
        sprite_object_.setScale(scale_.x, scale_.y);
    }
}
//------------------------------------------------------------------------------------------------------------