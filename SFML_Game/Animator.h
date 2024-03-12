#pragma once

#include <iostream>
#include <vector>
#include <list>

#include "AssetManager.h"

//------------------------------------------------------------------------------------------------------------
class Animator
{
public:

	struct Animation
	{
		std::string name;
		std::string texture_name;

		std::vector<sf::IntRect> frames;

		sf::Time duration;

		bool looping;

		Animation(std::string const& name, std::string const& textrueName,
			sf::Time const& duration, bool looping) : name(name), texture_name(textrueName),
			duration(duration), looping(looping)
		{
		}
		void addFrames(sf::Vector2i const& startFrom, sf::Vector2i const& frameSize, sf::Vector2i const& framesOffset,
			unsigned int framesNumber, unsigned int traccia)
		{
			sf::Vector2i current = startFrom;
			for (unsigned int i = 0; i < traccia; i++)
			{
				for (unsigned int j = 0; j < framesNumber; j++)
				{
					frames.push_back(sf::IntRect(current.x, current.y, frameSize.x, frameSize.y));

					current.x += framesOffset.x;
				}
				current.y += framesOffset.y;
				current.x = startFrom.x;
			}
		}
	};

	explicit Animator(sf::Sprite& sprite);

	Animator::Animation& createAnimation(std::string const& name,
		std::string const& textureName, sf::Time const& duration,
		bool loop = false);

	void update(sf::Time const& dt);
	bool switchAnimation(std::string const& name);
	void restart();

	std::string getCurrentAnimationName() const;
	bool getEndAnim() const;
	const int getCurrentFrame() const;

private:
	Animator::Animation* findAnimation(std::string const& name);

	void switchAnimation(Animator::Animation* animation);

	std::list<Animator::Animation> Animations_;

	Animator::Animation* CurrentAnimation_;

	sf::Sprite& sprite_;

	sf::Time current_time_;

	bool end_anim_;

	unsigned short current_frame_;
};
//------------------------------------------------------------------------------------------------------------
