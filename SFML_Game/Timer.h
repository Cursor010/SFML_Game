#pragma once

#include <sstream>
#include <iomanip>

#include "TextRenderer.h"
#include "Sound.h"

//------------------------------------------------------------------------------------------------------------
class Timer
{
public:
    Timer(const unsigned short countdown, const unsigned int size);

    bool update(bool pause, Sound& sound);

    const std::unique_ptr<TextRenderer>& getText() const;
    const std::string getTime() const;

private:
    bool updateTimer(bool pause);
    bool start(bool pause, Sound& sound);

    std::unique_ptr<TextRenderer> Text_;

    std::string time_;

    sf::Clock start_clock_;
    sf::Clock timer_clock_;

    sf::Time total_time_;

    bool is_go_display_;
    bool game_started_;
    bool is_active_;

    unsigned int count_down_;
};
//------------------------------------------------------------------------------------------------------------