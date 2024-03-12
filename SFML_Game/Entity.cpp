#include "Entity.h"

// Entity
//------------------------------------------------------------------------------------------------------------
Entity::Entity(const sf::RenderWindow& window, const sf::Vector2f& pos, std::string const& texture, float time, const sf::Vector2i& spriteSize, const sf::Vector2i& framesOffset, const unsigned int frame)
	: window_(window), pos_(pos)
{
	sprite_object_.setPosition(pos.x, pos.y);

	Borders_.max_x = static_cast<float>(window.getSize().x);
	Borders_.max_y = static_cast<float>((720 * window.getSize().y) / 810);
}
//------------------------------------------------------------------------------------------------------------
void Entity::setStepx(float x)
{
	stepx_ = x;
}
//------------------------------------------------------------------------------------------------------------
void Entity::setStepy(float y)
{
	stepy_ = y;
}
//------------------------------------------------------------------------------------------------------------
void Entity::setBordersObject(float min_x, float min_y, float max_x, float max_y)
{
	Borders_.min_x = min_x;
	Borders_.min_y = min_y;
	Borders_.max_x = max_x;
	Borders_.max_y = max_y;
}
//------------------------------------------------------------------------------------------------------------
void Entity::setDead(bool objectdead)
{
	dead_ = objectdead;
}
//------------------------------------------------------------------------------------------------------------
void Entity::setSpritePosition(float x, float y)
{
	sprite_object_.setPosition(x, y);
}
//------------------------------------------------------------------------------------------------------------
const bool Entity::getDead() const
{
	return dead_;
}
//------------------------------------------------------------------------------------------------------------
const sf::Sprite& Entity::getSpriteObject() const
{
	return sprite_object_;
}
//------------------------------------------------------------------------------------------------------------
