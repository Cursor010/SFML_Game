#pragma once

#include <array>

#include "Common.h"
#include "Entity.h"
#include "Map.h"

//------------------------------------------------------------------------------------------------------------
class AssistanceResources : public Entity
{
public:
	AssistanceResources(const sf::RenderWindow& window, const sf::Vector2f& pos, std::string const& texture, float time, const sf::Vector2i& spriteSize, const sf::Vector2i& framesOffset, const unsigned int frame);

	enum struct State { MOVE_UP, MOVE_DOWN };

	void update(sf::Time const& deltaTime) override;

	void setState(State state, float step);
	void setDead(bool objectdead) override;
	void setOnGroundState(bool stground);


	static std::unique_ptr<AssistanceResources> getResource(const sf::RenderWindow& window, const std::string& texture, float time, const sf::Vector2i& spriteSize, const sf::Vector2i& framesOffset, const unsigned int frame);

	bool checkInteractions(sf::Time const& deltaTime, const sf::Sprite& player, const sf::Sprite& resource, const unsigned short healthValue);

private:
	Map Map_;

	sf::Vector2f window_size_;

	bool on_ground_;
};
//------------------------------------------------------------------------------------------------------------
