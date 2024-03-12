#pragma once

#include<array>

#include "AssetManager.h"

//------------------------------------------------------------------------------------------------------------
class Sound
{
public:
	Sound();

	enum struct Sounds { GO, ONE, TWO, THREE, BLOWUP, PICKUP, CROUCH, JUMP, BUTTON, BUTTONRETURN };

	void play(Sounds sounds);
	void stop(Sounds sounds);
	void stopAll();

	void setVolume(float volume);

	const bool getOnOffSound() const;

private:
	std::array<sf::Sound, 10> GameSounds_;
};
//------------------------------------------------------------------------------------------------------------
