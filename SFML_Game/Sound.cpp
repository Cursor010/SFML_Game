#include "Sound.h"

// Sound
//------------------------------------------------------------------------------------------------------------
Sound::Sound()
{
	std::array<std::string, 10> name_file_buf{ "../audio/go.wav", "../audio/1.wav", "../audio/2.wav", "../audio/3.wav", "../audio/blowUp.wav", "../audio/pickUp.wav", "../audio/playerCrouch.wav", "../audio/playerJump.wav",
		 "../audio/button.wav", "../audio/buttonReturn.wav" };

	for (int i = 0; i < 10; i++)
	{
		GameSounds_[i].setBuffer(AssetManager::GetSoundBuffer(name_file_buf[i]));
	}
};
//------------------------------------------------------------------------------------------------------------
void Sound::setVolume(float volume)
{
	for (auto& sound : GameSounds_)
	{
		sound.setVolume(volume);
	}
}
//------------------------------------------------------------------------------------------------------------
void Sound::play(Sounds sounds)
{
	if (GameSounds_[static_cast<int>(sounds)].getStatus() == sf::SoundSource::Status::Stopped)
	{
		GameSounds_[static_cast<int>(sounds)].play();
	}

}
//------------------------------------------------------------------------------------------------------------
void Sound::stop(Sounds sounds)
{
	if (GameSounds_[static_cast<int>(sounds)].getStatus() == sf::SoundSource::Status::Playing)
	{
		GameSounds_[static_cast<int>(sounds)].stop();
	}
}
//------------------------------------------------------------------------------------------------------------
void Sound::stopAll()
{
	for (auto& sound : GameSounds_)
	{
		if (sound.getStatus() == sf::SoundSource::Status::Playing)
		{
			sound.stop();
		}
	}
}
//------------------------------------------------------------------------------------------------------------