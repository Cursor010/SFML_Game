#pragma once

#include "Common.h"
#include "TextRenderer.h"

//------------------------------------------------------------------------------------------------------------
class Menu
{
public:
	Menu(sf::RenderWindow& window, float menux, float menuy, unsigned int sizeFont, unsigned int step, std::vector<sf::String>& name);
    
    void updateMenuAppearance(int selected);
    void draw();

    void moveUp();
    void moveDown();

    int getSelectedMenuNumber() const;

private:
    sf::RenderWindow &window_;

    std::array<std::unique_ptr<TextRenderer>, 4> MainMenu_;

    sf::Color selection_color_;
    sf::Color chose_color_;
    sf::Color menu_color_;

    unsigned short main_menu_selected_;
    unsigned short max_menu_;
    unsigned short menu_step_;

    float menu_x_;
    float menu_y_;
};
//------------------------------------------------------------------------------------------------------------

