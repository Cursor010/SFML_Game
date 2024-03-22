#include "Engine.h"

// Enginee
//------------------------------------------------------------------------------------------------------------
Engine::Engine()
	: pause_(false)
{
	sound_volume_ = std::stof(FileHandler_->readFile("../save/info.txt", 3));
	music_volume_ = std::stof(FileHandler_->readFile("../save/info.txt", 2));

	menu_music_.openFromFile("../music/menu.ogg");
	menu_music_.setLoop(true);

	menu_music_.setVolume(music_volume_);
	game_music_.setVolume(music_volume_);

	Sound_ = std::make_unique<Sound>();

	Sound_->setVolume(sound_volume_);
}
//------------------------------------------------------------------------------------------------------------
void Engine::input()
{
	sf::Event event;

	while (game_window_->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			game_window_->close();
		}

		if (event.type == sf::Event::KeyPressed)
		{
			handleKeyPress(event);
		}

		if (event.type == sf::Event::KeyReleased)
		{
			handleKeyRelease(event);
		}
	}
}
//------------------------------------------------------------------------------------------------------------
void Engine::handleKeyPress(const sf::Event& event)
{
	sf::Sprite player = Player_->getSpriteObject();
	switch (event.key.code)
	{
	case sf::Keyboard::Escape:
		game_music_.setPitch(0.8f);
		pause_ = true;
		Pause();
		break;

	case sf::Keyboard::Right:
		if (!Player_->getRepulseState())
		{
			Player_->setState(Player::State::MOVE_RIGHT);
		}
		break;

	case sf::Keyboard::Left:
		if (!Player_->getRepulseState())
		{
			Player_->setState(Player::State::MOVE_LEFT);
		}
		break;

	case sf::Keyboard::Up:
		if (Player_->getOnGroundState() && !Player_->getSquatState())
		{
			float stepx = Player_->getSteps().x;
			Sound_->play(Sound::Sounds::JUMP);

			if (stepx > 0)
			{
				Player_->setStepx(0);
				Player_->setState(Player::State::MOVE_RIGHT, 0.5f);
			}

			else if (stepx < 0)
			{
				Player_->setStepx(0);
				Player_->setState(Player::State::MOVE_LEFT, 0.5f);
			}
			Player_->setJumpState(true);
			Player_->setLastPosition(static_cast<float>(Player_->getSpriteObject().getPosition().y));
			Player_->setOnGroundState(false);
		}
		break;

	case sf::Keyboard::Down:
		if (Player_->getOnGroundState() && !Player_->getRepulseState())
		{
			if (!Player_->getSquatState())
			{
				Sound_->play(Sound::Sounds::CROUCH);
			}
			Player_->setState(Player::State::SQUAT);
			Player_->setSquatState(true);
		}
		break;

	default:
		break;
	}
}
//------------------------------------------------------------------------------------------------------------
void Engine::handleKeyRelease(const sf::Event& event)
{
	switch (event.key.code)
	{
	case sf::Keyboard::Right:
		Player_->setStepx(0);
		break;

	case sf::Keyboard::Left:
		Player_->setStepx(0);
		break;

	case sf::Keyboard::Up:
		Player_->setStepy(0);
		break;

	case sf::Keyboard::Down:

		Player_->setStepy(0);
		Player_->setSquatState(false);
		break;

	default:
		break;
	}
}
//------------------------------------------------------------------------------------------------------------
void Engine::update(sf::Time const& deltaTime)
{
	Player_->update(deltaTime);
	Health_->update(deltaTime);

	if (Explosion_ != nullptr && !Explosion_->getDead())
	{
		Explosion_->update(deltaTime);
	}

	if (Player_->getOnGroundState() && !Player_->getSquatState() && Player_->getSteps().x == 0 && Player_->getSteps().y == 0)
	{
		Player_->setState(Player::State::STAY);
	}

	updateEnemies(deltaTime);
	updateResource(deltaTime);

	tm_ += deltaTime;

	if (tm_ > sf::milliseconds(3))
	{
		Timer_->getText()->align(static_cast<float>(game_window_->getSize().x), 0);

		if (Timer_->update(pause_, *Sound_))
		{
			for (const auto& resource : HealResources_)
			{
				resource->setState(AssistanceResources::State::MOVE_DOWN, 0.5f);
			}
			for (const auto& enemy : Enemies_)
			{
				Enemy::State state = enemy->getMovement() ? Enemy::State::MOVE_RIGHT : Enemy::State::MOVE_LEFT;
				enemy->setState(state, 1.7f);
			}
			tm_ = sf::milliseconds(0);
		}
	}
}
//------------------------------------------------------------------------------------------------------------
void Engine::updateEnemies(sf::Time const& deltaTime)
{
	for (const auto& enemy : Enemies_)
	{
		enemy->update(deltaTime);
		if (enemy->checkInteractions(*Player_, *Health_, enemy->getSpriteObject(), deltaTime))
		{
			if (!Player_->getSquatState())
			{
				Player_->setRepulseState(true);
				Player_->setStepy(0);

				Player_->setState(enemy->getMovement() ? Player::State::MOVE_RIGHT : Player::State::MOVE_LEFT, 0.5f);

				Player_->setJumpState(true);
				Player_->setLastPosition(static_cast<float>(Player_->getSpriteObject().getPosition().y));
				Player_->setOnGroundState(false);
			}

			Player_->setState(Player::State::STAY);
			if (enemy->getMovement())
			{
				Player_->setState(Player::State::MOVE_RIGHT, 0.5f);
			}
			else
			{
				Player_->setState(Player::State::MOVE_LEFT, 0.5f);
			}

			Player_->setJumpState(true);
			Player_->setLastPosition(static_cast<float>(Player_->getSpriteObject().getPosition().y));
			Player_->setOnGroundState(false);

			Explosion_ = std::make_unique<Explosion>(*game_window_, sf::Vector2f(Player_->getSpriteObject().getPosition().x, Player_->getSpriteObject().getPosition().y), "../images/explo.png", 0.5f, sf::Vector2i(350, 515), sf::Vector2i(515, 1), 11);

			enemy->setDead(true);

			Sound_->play(Sound::Sounds::BLOWUP);

			if (Health_->getHealth() == 1)
			{
				Player_->setDead(true);

				if (std::stof(FileHandler_->readFile("../save/info.txt", 1)) < std::stof(Timer_->getTime()))
				{
					FileHandler_->writeFile("../save/info.txt", Timer_->getTime(), 1);
				}

				Health_->setHealth();
				Explosion_->setDead(true);
				game_music_.pause();
				menu_music_.play();
				game_window_->close();
				GameMenu();
			}

			Health_->setHealth(Health_->getHealth() - 1);
		}
	}
}
//------------------------------------------------------------------------------------------------------------
void Engine::updateResource(sf::Time const& deltaTime)
{
	for (auto& resource : HealResources_)
	{
		resource->update(deltaTime);
		if (resource->checkInteractions(deltaTime, Player_->getSpriteObject(), resource->getSpriteObject(), Health_->getHealth()))
		{
			Sound_->play(Sound::Sounds::PICKUP);
			resource->setDead(true);
			Health_->setHealth(Health_->getHealth() + 1);
		}
	}

}
//------------------------------------------------------------------------------------------------------------
void Engine::draw()
{
	game_window_->clear();

	game_window_->draw(background_);
	game_window_->draw(Player_->getSpriteObject());
	game_window_->draw(map_);
	game_window_->draw(Health_->getSpriteObject());
	Timer_->getText()->draw(*game_window_);

	for (const auto& enemy : Enemies_)
	{
		game_window_->draw(enemy->getSpriteObject());
	}

	for (const auto& resource : HealResources_)
	{
		game_window_->draw(resource->getSpriteObject());
	}

	if (Explosion_ != nullptr && !Explosion_->getDead())
	{
		game_window_->draw(Explosion_->getSpriteObject());
	}

	game_window_->display();
}
//------------------------------------------------------------------------------------------------------------
void Engine::run()
{
	menu_music_.play();
	GameMenu();
}
//------------------------------------------------------------------------------------------------------------
void Engine::GameMenu()
{
	sf::RenderWindow menu_window;

	sf::Clock clock;
	sf::Time tm;

	auto width = static_cast<float>(sf::VideoMode::getDesktopMode().width);
	auto height = static_cast<float>(sf::VideoMode::getDesktopMode().height);

	menu_window.create(sf::VideoMode::getDesktopMode(), L"DEATH GUMP", sf::Style::Fullscreen);

	SetWindowLong(menu_window.getSystemHandle(), GWL_EXSTYLE, GetWindowLong(menu_window.getSystemHandle(), GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(menu_window.getSystemHandle(), 0, 0, LWA_COLORKEY);
	menu_window.setMouseCursorVisible(false);

	sf::RectangleShape homecls(sf::Vector2f(640, 280));
	homecls.setTexture(&AssetManager::GetTexture("../images/loading.png"));
	homecls.setPosition(sf::Vector2f(width / 2 - homecls.getLocalBounds().width / 2, height / 2 - homecls.getLocalBounds().height / 2));

	std::vector<sf::String> name_menu{ L"START", L"SETTINGS", L"ABOUT", L"EXIT" };
	Menu menu(menu_window, 0, (300 * height) / 720, static_cast<int>((37 * width) / 1280), static_cast<int>((67 * height) / 720), name_menu);

	sf::RectangleShape background(sf::Vector2f(width, height));
	background.setTexture(&AssetManager::GetTexture("../images/menu.png"));

	std::unique_ptr<TextRenderer> titul = std::make_unique<TextRenderer>(std::string("../fonts/rs.ttf"), static_cast<unsigned int>((97 * width) / 1280),
		sf::Vector2f(0.0f, 10.0f), std::string("DEATHJUMP!"), sf::Color(255, 255, 255));
	titul->align(static_cast<float>(width), 0);

	std::stringstream score;
	score << std::fixed << std::setprecision(2) << FileHandler_->readFile("../save/info.txt", 1);

	std::unique_ptr<TextRenderer> best_score = std::make_unique<TextRenderer>("../fonts/rs.ttf", static_cast<unsigned int>((40 * width) / 1280), sf::Vector2f(0.0f, (220 * height) / 720), "BEST TIME: " + score.str(), sf::Color(255, 255, 255));
	best_score->align(static_cast<float>(width), 0);
	best_score->setSelection(sf::Vector2f(30, 20), sf::Color(237, 211, 97, 128), 3, sf::Color(235, 196, 21));

	SetLayeredWindowAttributes(menu_window.getSystemHandle(), 100, 0, LWA_COLORKEY);

	menu_window.draw(homecls);
	menu_window.display();

	while (menu_window.isOpen())
	{
		sf::Event event;
		sf::Time deltaTime = clock.restart();
		tm += deltaTime;

		while (menu_window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::Up)
				{
					Sound_->play(Sound::Sounds::BUTTON);
					menu.moveUp();
				}
				if (event.key.code == sf::Keyboard::Down)
				{
					Sound_->play(Sound::Sounds::BUTTON);
					menu.moveDown();
				}
				if (event.key.code == sf::Keyboard::Enter)
				{
					Sound_->play(Sound::Sounds::BUTTONRETURN);

					switch (menu.getSelectedMenuNumber())
					{
					case 0:
						menu_window.close();
						menu_music_.pause();
						game_music_.play();
						GameStart();
						break;

					case 1:
						menu_window.close();
						Options();
						break;

					case 2:
						menu_window.close();
						AboutGame();
						break;

					case 3:
						menu_window.close();
						break;

					default:
						break;
					}
				}
			}
		}

		if (tm > sf::milliseconds(3))
		{
			titul->setLimitMovement(0.1f, 10.f, 10);
			tm = sf::milliseconds(0);
		}

		menu_window.clear();
		menu_window.draw(background);
		titul->draw(menu_window);
		best_score->draw(menu_window);
		menu.draw();
		menu_window.display();
	}
}
//------------------------------------------------------------------------------------------------------------
void Engine::Options()
{
	sf::RenderWindow options_window;

	auto width = static_cast<float>(sf::VideoMode::getDesktopMode().width);
	auto height = static_cast<float>(sf::VideoMode::getDesktopMode().height);

	const unsigned short sound_step_v = 20;
	const unsigned short music_step_v = 5;
	const unsigned short sound_current_v = std::stoi(FileHandler_->readFile("../save/info.txt", 3)) / sound_step_v;
	const unsigned short music_current_v = std::stoi(FileHandler_->readFile("../save/info.txt", 2)) / music_step_v;

	options_window.create(sf::VideoMode::getDesktopMode(), L"Settings", sf::Style::Fullscreen);

	sf::RectangleShape background_opt(sf::Vector2f(static_cast<float>(sf::VideoMode::getDesktopMode().width), static_cast<float>(sf::VideoMode::getDesktopMode().height)));
	background_opt.setTexture(&AssetManager::GetTexture("../images/menu.png"));

	std::unique_ptr<TextRenderer> sound_titul = std::make_unique<TextRenderer>(std::string("../fonts/rs.ttf"), static_cast<unsigned int>((50 * options_window.getSize().x) / 1280),
		sf::Vector2f(0.0f, (height * 13) / 720), std::string("SOUND VOLUME"), sf::Color(255, 255, 255));
	sound_titul->align(static_cast<float>(options_window.getSize().x), 0);

	std::unique_ptr<TextRenderer> music_titul = std::make_unique<TextRenderer>(std::string("../fonts/rs.ttf"), static_cast<unsigned int>((50 * options_window.getSize().x) / 1280),
		sf::Vector2f(0.0f, (height * 200) / 720), std::string("MUSIC VOLUME"), sf::Color(255, 255, 255));
	music_titul->align(static_cast<float>(options_window.getSize().x), 0);

	std::unique_ptr<TextRenderer> player_titul = std::make_unique<TextRenderer>(std::string("../fonts/rs.ttf"), static_cast<unsigned int>((50 * options_window.getSize().x) / 1280),
		sf::Vector2f(0.0f, (height * 400) / 720), std::string("PLAYER PARTICLE"), sf::Color(255, 255, 255));
	player_titul->align(static_cast<float>(options_window.getSize().x), 0);

	std::unique_ptr<OptionsBar> sound_bar = std::make_unique<OptionsBar>(static_cast<float>(options_window.getSize().x), sound_current_v, (65 * width) / 1280, 10, (60 * width) / 1280, (height * 93) / 720, true);
	std::unique_ptr<OptionsBar> music_bar = std::make_unique<OptionsBar>(static_cast<float>(options_window.getSize().x), music_current_v, (65 * width) / 1280, 10, (60 * width) / 1280, (height * 280) / 720, false);

	while (options_window.isOpen())
	{
		sf::Event event_opt;
		while (options_window.pollEvent(event_opt))
		{
			if (event_opt.type == sf::Event::Closed)
			{
				options_window.close();
			}
			else if (event_opt.type == sf::Event::KeyReleased)
			{
				switch (event_opt.key.code)
				{
				case sf::Keyboard::Left:
					if (sound_bar->getState())
					{
						if (sound_bar->reduce())
						{
							Sound_->play(Sound::Sounds::BUTTON);
							sound_volume_ -= sound_step_v;
						}
					}
					else
					{
						if (music_bar->reduce())
						{
							music_volume_ -= music_step_v;
						}
					}
					break;

				case sf::Keyboard::Right:
					if (sound_bar->getState())
					{
						if (sound_bar->increase())
						{
							Sound_->play(Sound::Sounds::BUTTON);
							sound_volume_ += sound_step_v;
						}
					}
					else
					{
						if (music_bar->increase())
						{
							music_volume_ += music_step_v;
						}
					}
					break;

				case sf::Keyboard::Up:
					if (!sound_bar->getState())
					{
						sound_bar->setState(true);
						music_bar->setState(false);
					}
					break;

				case sf::Keyboard::Down:
					if (!music_bar->getState())
					{
						music_bar->setState(true);
						sound_bar->setState(false);
					}
					break;

				case sf::Keyboard::Escape:
					options_window.close();
					if (!pause_)
					{
						GameMenu();
					}
					break;

				default:
					break;
				}

				Sound_->setVolume(sound_volume_);
				menu_music_.setVolume(music_volume_);
				game_music_.setVolume(music_volume_);

				FileHandler_->writeFile("../save/info.txt", std::to_string(sound_volume_), 3);
				FileHandler_->writeFile("../save/info.txt", std::to_string(music_volume_), 2);
			}
		}

		options_window.clear();
		options_window.draw(background_opt);
		sound_bar->draw(options_window);
		music_bar->draw(options_window);
		sound_titul->draw(options_window);
		music_titul->draw(options_window);
		player_titul->draw(options_window);
		options_window.display();
	}
}
//------------------------------------------------------------------------------------------------------------
void Engine::AboutGame()
{
	sf::RenderWindow about_window;
	about_window.create(sf::VideoMode::getDesktopMode(), L"About Game", sf::Style::Fullscreen);

	sf::RectangleShape background_ab(sf::Vector2f(static_cast<float>(sf::VideoMode::getDesktopMode().width), static_cast<float>(sf::VideoMode::getDesktopMode().height)));
	background_ab.setTexture(&AssetManager::GetTexture("../images/menu.png"));

	while (about_window.isOpen())
	{
		sf::Event event_opt;
		while (about_window.pollEvent(event_opt))
		{
			if (event_opt.type == sf::Event::Closed) about_window.close();
			if (event_opt.type == sf::Event::KeyPressed)
			{
				if (event_opt.key.code == sf::Keyboard::Escape)
				{
					about_window.close();
					if (!pause_)
					{
						GameMenu();
					}
				}
			}
		}
		about_window.clear();
		about_window.draw(background_ab);
		about_window.display();
	}
}
//------------------------------------------------------------------------------------------------------------
void Engine::GameStart()
{
	auto width = static_cast<float>(sf::VideoMode::getDesktopMode().width);
	auto height = static_cast<float>(sf::VideoMode::getDesktopMode().height);

	game_window_ = std::make_unique<sf::RenderWindow>(sf::VideoMode::getDesktopMode(), L"DEATH GUMP", sf::Style::Fullscreen);

	Timer_ = std::make_unique<Timer>(3, static_cast<int>((60 * width) / 1280));
	Health_ = std::make_unique<Health>(*game_window_, sf::Vector2f(static_cast<float>(width / 30), static_cast<float>(height / 16)), "../images/health.png", 0.5f, sf::Vector2i(96, 22), sf::Vector2i(0, 10), 3);
	Player_ = std::make_unique<Player>(*game_window_, sf::Vector2f(static_cast<float>(width / 2), static_cast<float>((720 * height) / 810)), "../images/player.psd", 0.5f, sf::Vector2i(23, 40), sf::Vector2i(96, 44), 7);

	background_ = sf::RectangleShape(sf::Vector2f(static_cast<float>(width), static_cast<float>(height)));
	background_.setTexture(&AssetManager::GetTexture("../images/background.png"));

	map_ = sf::RectangleShape(sf::Vector2f(static_cast<float>(width), static_cast<float>(height)));
	map_.setTexture(&AssetManager::GetTexture("../images/map.png"));

	for (size_t i = 0; i < Enemies_.size(); i++)
	{
		Enemies_[i] = Enemy::getEnemies(*game_window_, "../images/fireball.psd", 0.5f, sf::Vector2i(65, 31), sf::Vector2i(65, 0), 4);
	}

	for (size_t i = 0; i < size(HealResources_); i++)
	{
		HealResources_[i] = AssistanceResources::getResource(*game_window_, "../images/heart.png", 0.5f, sf::Vector2i(32, 32), sf::Vector2i(32, 0), 4);
	}

	game_music_.openFromFile("../music/main.ogg");
	game_music_.setLoop(true);
	game_music_.setVolume(music_volume_);

	game_music_.setPitch(1.f);
	game_music_.play();

	sf::Clock clock;
	while (game_window_->isOpen())
	{
		sf::Time dt;
		dt = clock.restart();

		if (!pause_)
		{
			input();
			update(dt);
			draw();
		}
	}
}
//------------------------------------------------------------------------------------------------------------
void Engine::Pause()
{
	sf::RenderWindow pause_window;
	pause_window.create(sf::VideoMode::getDesktopMode(), L"PAUSE", sf::Style::Fullscreen);

	sf::Clock clock;
	sf::Time tm;

	auto width = static_cast<float>(sf::VideoMode::getDesktopMode().width);
	auto height = static_cast<float>(sf::VideoMode::getDesktopMode().height);

	SetWindowLong(pause_window.getSystemHandle(), GWL_EXSTYLE, GetWindowLong(pause_window.getSystemHandle(), GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(pause_window.getSystemHandle(), 0, 0, LWA_COLORKEY);
	pause_window.setMouseCursorVisible(false);

	std::vector<sf::String> name_menu{ L"RESUME", L"SETTINGS", L"EXIT TO MENU" };
	Menu menu_window(pause_window, 0, static_cast<float>((300 * height) / 720), static_cast<int>((37 * width) / 1280), static_cast<int>((67 * height) / 720), name_menu);

	sf::RectangleShape background(sf::Vector2f(width, height));
	background.setTexture(&AssetManager::GetTexture("../images/menu.png"));

	std::unique_ptr<TextRenderer> titul = std::make_unique<TextRenderer>("../fonts/rs.ttf", static_cast<int>((97 * width) / 1280), sf::Vector2f(0.0f, 10.0f), "PAUSED", sf::Color(255, 255, 255));
	titul->align(static_cast<float>(width), 0);

	SetLayeredWindowAttributes(pause_window.getSystemHandle(), 100, 0, LWA_COLORKEY);
	while (pause_window.isOpen())
	{
		sf::Event event;
		sf::Time deltaTime = clock.restart();
		tm += deltaTime;

		while (pause_window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::Up)
				{
					Sound_->play(Sound::Sounds::BUTTON);
					menu_window.moveUp();
				}
				if (event.key.code == sf::Keyboard::Down)
				{
					Sound_->play(Sound::Sounds::BUTTON);
					menu_window.moveDown();
				}
				if (event.key.code == sf::Keyboard::Enter)
				{
					Sound_->play(Sound::Sounds::BUTTONRETURN);

					switch (menu_window.getSelectedMenuNumber())
					{
					case 0:
						pause_window.close();
						game_music_.setPitch(1.f);
						pause_ = false;
						break;

					case 1:
						Options();
						break;

					case 2:
						pause_window.close();
						game_window_->close();
						pause_ = false;
						game_music_.pause();
						menu_music_.play();

						if (Timer_->getTime() != "0")
						{
							if (std::stof(FileHandler_->readFile("../save/info.txt", 1)) < std::stof(Timer_->getTime()))
							{
								FileHandler_->writeFile("../save/info.txt", Timer_->getTime(), 1);
							}
						}
						GameMenu();
						break;

					default:
						break;
					}
				}
			}
		}

		if (tm > sf::milliseconds(3))
		{
			Timer_->update(pause_, *Sound_);

			titul->setLimitMovement(0.1f, 10.f, 10);
			tm = sf::milliseconds(0);
		}

		pause_window.clear();
		pause_window.draw(background);
		titul->draw(pause_window);
		menu_window.draw();
		pause_window.display();
	}
}
//------------------------------------------------------------------------------------------------------------
