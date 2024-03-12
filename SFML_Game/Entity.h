#pragma once

#include "Animator.h"

// Entity
//------------------------------------------------------------------------------------------------------------
class Entity
{
public:
	Entity(const sf::RenderWindow& window, const sf::Vector2f& pos, std::string const& texture, float time, const sf::Vector2i& spriteSize, const sf::Vector2i& framesOffset, const unsigned int frame);

	virtual void update(sf::Time const& deltaTime) = 0;

	virtual void setStepx(float x);
	virtual void setStepy(float y);
	virtual void setBordersObject(float min_x, float min_y, float max_x, float max_y);
	virtual void setDead(bool objectdead);
	virtual void setSpritePosition(float x, float y);

	virtual const bool getDead() const;
	virtual const sf::Sprite& getSpriteObject() const;

protected:
	struct Borders
	{
		float min_x;
		float min_y;
		float max_x;
		float max_y;
	};

	Animator AnimObject_ = Animator(sprite_object_);

	Borders Borders_{ 0 };

	const sf::RenderWindow& window_;

	sf::Sprite sprite_object_;

	sf::Vector2f pos_;

	sf::Time time_update_;

	bool dead_ = false;

	float stepx_ = 0.0f;
	float stepy_ = 0.0f;
};
//------------------------------------------------------------------------------------------------------------

