#pragma once

#include "Entity.h"
#include "Common.h"
#include "Explosion.h"
#include "Health.h"
#include "Player.h"

//------------------------------------------------------------------------------------------------------------
class Enemy : public Entity
{
public:
	Enemy(const sf::RenderWindow& window, const sf::Vector2f& pos, std::string const& texture, float time, const sf::Vector2i& spriteSize, const sf::Vector2i& framesOffset, const unsigned int frame);

	enum struct State { MOVE_RIGHT, MOVE_LEFT };

	void update(sf::Time const& deltaTime) override;
	void updatePosition();

	void setDead(bool objectdead) override;
	void setState(State state, float step = 1.f);

	const bool getMovement() const;
	static std::unique_ptr<Enemy> getEnemies(const sf::RenderWindow& window, const std::string& texture, float time, const sf::Vector2i& spriteSize, const sf::Vector2i& framesOffset, const unsigned int frame);

	bool checkInteractions(Player& player, Health& health, sf::Sprite enemy, const sf::Time& deltaTime);

private:
	void reflectSprite(bool rightMovment);

	sf::Vector2f window_size_;
	sf::Vector2f scale_;

	bool right_movement_;
};
//------------------------------------------------------------------------------------------------------------
