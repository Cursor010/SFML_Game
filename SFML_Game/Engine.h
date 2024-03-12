#pragma once

#include "Animator.h"
#include "Enemy.h"
#include "AssistanceResources.h"
#include "Timer.h"
#include "Menu.h"
#include "Sound.h"
#include "OptionsBar.h"

#include <Windows.h>

//------------------------------------------------------------------------------------------------------------
class Engine
{
public:
	Engine();
	void run();
	void GameStart();
private:
	void input();
	void update(sf::Time const& deltaTime);
	void updateEnemies(sf::Time const& deltaTime);
	void updateResource(sf::Time const& deltaTime);
	void draw();

	void handleKeyPress(const sf::Event& event);
	void handleKeyRelease(const sf::Event& event);

	void GameMenu();
	void Options();
	void AboutGame();
	void Pause();

	AssetManager Manager_;

	std::unique_ptr<Health> Health_;
	std::unique_ptr<Player> Player_;
	std::unique_ptr<Explosion> Explosion_;
	std::unique_ptr<Timer> Timer_;
	std::unique_ptr<FileHandler> FileHandler_;
	std::unique_ptr<Sound> Sound_;

	std::array<std::unique_ptr<Enemy>, 4> Enemies_;
	std::array<std::unique_ptr<AssistanceResources>, 2> HealResources_;

	sf::Time tm_;

	sf::RectangleShape background_;
	sf::RectangleShape map_;
	std::unique_ptr<sf::RenderWindow> game_window_;

	sf::Music menu_music_;
	sf::Music game_music_;

	bool pause_;

	float sound_volume_;
	float music_volume_;
};
//------------------------------------------------------------------------------------------------------------