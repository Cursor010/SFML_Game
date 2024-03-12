#include "OptionsBar.h"

// OptionsBar
//------------------------------------------------------------------------------------------------------------
OptionsBar::OptionsBar(const float window_size_x, const unsigned short currentSel, const float offset, const unsigned short rectangleNumber, const float size, const float y_pos, const bool state)
	:current_(rectangleNumber - currentSel), rectangle_number_(rectangleNumber), active_(state), seletion_color_(sf::Color(119, 119, 119, 128)), backgorund_color_(sf::Color(215, 215, 215, 128))
{
	color_ = (active_) ? sf::Color(95, 95, 95) : sf::Color::White;

	for (size_t i = 0; i < rectangle_number_; ++i)
	{
		sf::RectangleShape rectangle(sf::Vector2f(size, size));

		if (i >= currentSel)
		{
			rectangle.setFillColor(backgorund_color_);
		}
		else
		{
			rectangle.setFillColor(color_);

		}
		rectangle.setPosition((window_size_x - offset * rectangleNumber) / 2 + offset * i, y_pos);
		rectangles_.push_back(rectangle);
	}
}
//------------------------------------------------------------------------------------------------------------
bool OptionsBar::increase()
{
	if (current_ > 0)
	{
		rectangles_[rectangle_number_ - current_].setFillColor(color_);
		current_--;
		return true;
	}

	return false;
}
//------------------------------------------------------------------------------------------------------------
bool OptionsBar::reduce()
{
	if (current_ < rectangle_number_)
	{
		rectangles_[rectangle_number_ - current_ - 1].setFillColor(backgorund_color_);
		current_++;
		return true;
	}

	return false;
}
//------------------------------------------------------------------------------------------------------------
void OptionsBar::draw(sf::RenderWindow& window)
{

	for (auto& rectangle : rectangles_)
	{
		window.draw(rectangle);
	}
}
//------------------------------------------------------------------------------------------------------------
void OptionsBar::setState(bool state)
{
	active_ = state;
	color_ = (active_) ? sf::Color(95, 95, 95) : sf::Color::White;

	for (size_t i = 0; i < rectangle_number_; ++i)
	{
		if (i >= rectangle_number_ - current_)
		{
			rectangles_[i].setFillColor(backgorund_color_);
		}
		else
		{
			rectangles_[i].setFillColor(color_);
		}
	}
}
//------------------------------------------------------------------------------------------------------------
const bool OptionsBar::getState() const
{
	return active_;
}
//------------------------------------------------------------------------------------------------------------


