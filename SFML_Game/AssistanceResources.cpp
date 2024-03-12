#include "AssistanceResources.h"

// AssistanceResources
//------------------------------------------------------------------------------------------------------------
AssistanceResources::AssistanceResources(const sf::RenderWindow& window, const sf::Vector2f& pos, std::string const& texture, float time, const sf::Vector2i& spriteSize, const sf::Vector2i& framesOffset, const unsigned int frame)
	: Entity(window, pos, texture, time, spriteSize, framesOffset, frame), on_ground_(false), Map_("../images/map1.png", sf::Vector2f(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)))
{
	window_size_.x = static_cast<float>(window.getSize().x);
	window_size_.y = static_cast<float>(window.getSize().y);

	sprite_object_.scale(static_cast<float>((1.1 * window_size_.x) / 1280), static_cast<float>((1.1 * window_size_.y) / 720));

	auto& anim_drop = AnimObject_.createAnimation("Drop", texture, sf::seconds(time), true);
	anim_drop.addFrames(sf::Vector2i(0, 0), spriteSize, framesOffset, frame, 1);
}
//------------------------------------------------------------------------------------------------------------
void AssistanceResources::update(sf::Time const& deltaTime)
{
	if (dead_)
	{
		return;
	}

	AnimObject_.update(deltaTime);

	time_update_ += deltaTime;

	if (time_update_ > sf::milliseconds(3))
	{
		time_update_ = sf::microseconds(0);

		sprite_object_.move(stepx_, stepy_);

		sf::Vector2f position = sprite_object_.getPosition();
		sf::FloatRect bounds = sprite_object_.getGlobalBounds();

		if (position.y > Borders_.max_y - bounds.height)
		{
			position.y = Borders_.max_y - bounds.height;
		}

		sprite_object_.setPosition(position);
	}
}
//------------------------------------------------------------------------------------------------------------
void AssistanceResources::setState(State state, float step)
{
	if (!dead_)
	{
		switch (state)
		{
		case State::MOVE_DOWN:
		{
			if (!on_ground_)
			{
				if (sprite_object_.getPosition().y < Borders_.max_y)
				{
					stepy_ = step * window_size_.y / 720;
				}
			}
			break;
		}
		default:
			break;
		}
	}
}
//------------------------------------------------------------------------------------------------------------
void AssistanceResources::setDead(bool objectdead)
{
	RandomNumberGenerator gen;
	float x_pos = gen.getRandom(static_cast<int>(window_size_.x / 1280), static_cast<int>((window_size_.x * 10) / 1280)) * 120;
	float y_pos = gen.getRandom(static_cast<int>((window_size_.y * 300) / 720), static_cast<int>((window_size_.y * 700) / 720)) * -10;

	sprite_object_.setPosition(x_pos, y_pos);

	on_ground_ = false;
}
//------------------------------------------------------------------------------------------------------------
std::unique_ptr<AssistanceResources> AssistanceResources::getResource(const sf::RenderWindow& window, const std::string& texture, float time, const sf::Vector2i& spriteSize, const sf::Vector2i& framesOffset, const unsigned int frame)
{
	RandomNumberGenerator gen;
	float x_pos = gen.getRandom(window.getSize().x / 1280, (window.getSize().x * 10) / 1280) * 120;
	float y_pos = gen.getRandom((window.getSize().y * 300) / 720, (window.getSize().y * 700) / 720) * -10;

	return std::make_unique<AssistanceResources>(window, sf::Vector2f(x_pos, y_pos), texture, time, spriteSize, framesOffset, frame);
}
//------------------------------------------------------------------------------------------------------------
bool AssistanceResources::checkInteractions(sf::Time const& deltaTime, const sf::Sprite& player, const sf::Sprite& resource, const unsigned short healthValue)
{
	sf::FloatRect player_bounds = player.getGlobalBounds();
	sf::FloatRect resource_bounds = resource.getGlobalBounds();

	const unsigned short health_value = healthValue;

	bool resource_intersects = resource_bounds.intersects(player_bounds);

	Map_.checkInteractions(sprite_object_, on_ground_);

	if (resource_intersects && health_value < 3)
	{
		return 1;
	}

	return 0;
}
//------------------------------------------------------------------------------------------------------------
void AssistanceResources::setOnGroundState(bool stground)
{
	on_ground_ = stground;
}
//------------------------------------------------------------------------------------------------------------
