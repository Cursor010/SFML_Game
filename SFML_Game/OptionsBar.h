#pragma once

#include <vector>

#include "TextRenderer.h"

//------------------------------------------------------------------------------------------------------------
class OptionsBar
{
public:
	OptionsBar(const float window_size_x, const unsigned short currentSel, const float offset, const unsigned short rectangleNumber, const float size, const float y_pos, const bool state);

	void draw(sf::RenderWindow& window);
	bool increase();
	bool reduce();

	void setState(bool state);

	const bool getState() const;

private:

	std::vector<sf::RectangleShape> rectangles_;

	sf::Color color_;
	sf::Color seletion_color_;
	sf::Color backgorund_color_;

	bool active_;

	unsigned short rectangle_number_;
	unsigned short current_;
};
//------------------------------------------------------------------------------------------------------------