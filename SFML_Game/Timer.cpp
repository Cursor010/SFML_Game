#include "Timer.h"

// Timer
//------------------------------------------------------------------------------------------------------------
Timer::Timer(const unsigned short countdown, const unsigned int size)
    : count_down_(countdown), is_go_display_(false), total_time_(sf::Time::Zero), game_started_(false), time_("0")
{
    Text_ = std::make_unique<TextRenderer>("../fonts/rs.ttf", size, sf::Vector2f(0.f, 10.0f), "", sf::Color(255, 255, 255));
    start_clock_.restart();
    timer_clock_.restart();
}
//------------------------------------------------------------------------------------------------------------
bool Timer::update(bool pause, Sound& sound)
{
    if (!game_started_)
    {
        return start(pause, sound);
    }
    else
    {
        return updateTimer(pause);
    }
}
//------------------------------------------------------------------------------------------------------------
bool Timer::start(bool pause, Sound& sound)
{
    sf::Time elapsed = start_clock_.getElapsedTime();

    if (pause)
    {
        timer_clock_.restart();
    }
    else
    {
        if (count_down_ > 0 && elapsed.asSeconds() >= 1)
        {
            Text_->setText(std::to_string(count_down_));
            sound.play(static_cast<Sound::Sounds>(count_down_));
            count_down_--;
            start_clock_.restart();
        }
        else if (count_down_ == 0 && elapsed.asSeconds() >= 1 && !is_go_display_)
        {
            Text_->setText("GO!");
            sound.play(Sound::Sounds::GO);
            is_go_display_ = true;
            start_clock_.restart();
            timer_clock_.restart();
            game_started_ = true;
        }
    }
    return false;
}
//------------------------------------------------------------------------------------------------------------
bool Timer::updateTimer(bool pause)
{
    sf::Time elapsed = timer_clock_.getElapsedTime();

    if (pause)
    {
        timer_clock_.restart();
    }
    else
    {
        unsigned int seconds = static_cast<int>(total_time_.asSeconds());
        unsigned int hundredths = static_cast<int>((total_time_.asMilliseconds() % 1000) / 10);

        total_time_ += timer_clock_.getElapsedTime();
        timer_clock_.restart();

        std::stringstream ss;
        ss << seconds << "." << std::setfill('0') << std::setw(2) << hundredths;

        time_ = ss.str();
        Text_->setText(time_);

        return true;
    }
    return false;
}
//------------------------------------------------------------------------------------------------------------
const std::unique_ptr<TextRenderer>& Timer::getText() const
{
    return Text_;
}
//------------------------------------------------------------------------------------------------------------
const std::string Timer::getTime() const
{
    return time_;
}
//------------------------------------------------------------------------------------------------------------