#pragma once

#include "Entity.h"
#include "Map.h"

//------------------------------------------------------------------------------------------------------------
class Player : public Entity
{
public:
	Player(const sf::RenderWindow& window, const sf::Vector2f& pos, std::string const& texture, float time, const sf::Vector2i& spriteSize, const sf::Vector2i& framesOffset, const unsigned int frame);

	enum struct State { MOVE_UP, MOVE_DOWN, MOVE_RIGHT, MOVE_LEFT, STAY, SQUAT };

	void update(sf::Time const& deltaTime) override;
	void updateStates();
	void toggleSpriteTransparency();

	void setState(State state, float step = 1.0f);
	void setSquatState(const bool stsquat);
	void setJumpState(const bool stjump);
	void setLastPosition(const float last_pos);
	void setOnGroundState(bool stground);
	void setRepulseState(bool strepulse);

	const sf::Vector2f getSteps() const;
	const bool getOnGroundState() const;
	const bool getSquatState() const;
	const bool getRepulseState() const;

private:
	void updatePosition();

	Map Map_;

	sf::Vector2f scale_;
	sf::Vector2f window_size_;

	bool is_squat_;
	bool is_jump_;
	bool is_repulse_;
	bool on_ground_;

	float jump_height_;
	float last_position_;
};
//------------------------------------------------------------------------------------------------------------
