#include "Menu.h"

// Menu
//------------------------------------------------------------------------------------------------------------
Menu::Menu(sf::RenderWindow& window, float menux, float menuy, unsigned int sizeFont, unsigned int step, std::vector<sf::String>& name)
    : menu_x_(menux), menu_y_(menuy), menu_step_(step), max_menu_(static_cast<int>(name.size())), window_(window),
    chose_color_(sf::Color(115, 115, 115)), menu_color_(sf::Color::White), selection_color_(sf::Color(115, 115, 115, 128))
{
    for (size_t i = 0, ypos = static_cast<int>(menu_y_); i < max_menu_; i++, ypos += menu_step_)
    {
        MainMenu_[i] = std::make_unique<TextRenderer>("../fonts/rs.ttf", sizeFont, sf::Vector2f(0, static_cast<float>(ypos)), name[i], menu_color_);
        MainMenu_[i]->align(static_cast<float>(window.getSize().x), 0.0f);
        MainMenu_[i]->setSelection(sf::Vector2f(20, 20), selection_color_, 3, menu_color_);
    }

    main_menu_selected_ = 0;
    updateMenuAppearance(main_menu_selected_);
}
//------------------------------------------------------------------------------------------------------------
void Menu::moveUp()
{
    main_menu_selected_ = (main_menu_selected_ - 1 + max_menu_) % max_menu_;
    updateMenuAppearance(main_menu_selected_);
}
//------------------------------------------------------------------------------------------------------------
void Menu::moveDown()
{
    main_menu_selected_ = (main_menu_selected_ + 1) % max_menu_;
    updateMenuAppearance(main_menu_selected_);
}
//------------------------------------------------------------------------------------------------------------
void Menu::updateMenuAppearance(int selected)
{
    for (size_t i = 0; i < max_menu_; i++)
    {
        MainMenu_[i]->setTextColor(i == selected ? chose_color_ : menu_color_);
        MainMenu_[i]->setSelectionColor(selection_color_, i == selected ? chose_color_ : menu_color_);
    }
}
//------------------------------------------------------------------------------------------------------------
void Menu::draw()
{
    for (size_t i = 0; i < max_menu_; i++)
    {
        MainMenu_[i]->draw(window_);
    }
}
//------------------------------------------------------------------------------------------------------------
int Menu::getSelectedMenuNumber() const
{
    return main_menu_selected_;
}
//------------------------------------------------------------------------------------------------------------