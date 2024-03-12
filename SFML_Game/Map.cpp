#include "Map.h"

// Map
//------------------------------------------------------------------------------------------------------------
Map::Map(std::string filePath, const sf::Vector2f& windowSize)
    :current_obstacle_(-1), window_size_(windowSize)
{
    createObstacle(filePath, 0.f, 955.f, 20.f);
    createObstacle(filePath, 120.f, 270.f, 1.8f);
    createObstacle(filePath, 360.f, 390.f, 2.7f);
    createObstacle(filePath, 0.f, 540.f, 1.5f);
    createObstacle(filePath, 420.f, 620.f, 2.1f);
    createObstacle(filePath, 690.f, 770.f, 1.6f);
    createObstacle(filePath, 925.f, 890.f, 0.8f, 2.4f);
    createObstacle(filePath, 120.f, 700.f, 2.3f);
    createObstacle(filePath, 920.f, 620.f, 2.7f);
    createObstacle(filePath, 920.f, 390.f, 1.2f);
    createObstacle(filePath, 730.f, 500.f, 1.2f);
    createObstacle(filePath, 1230.f, 780.f, 1.3f);
    createObstacle(filePath, 1300.f, 500.f, 2.3f);
    createObstacle(filePath, 1380.f, 200.f, 2.f);
    createObstacle(filePath, 1040.f, 270.f, 2.4f);
    createObstacle(filePath, 1040.f, 270.f, 1.f);
    createObstacle(filePath, 1690.f, 120.f, 2.f);
    createObstacle(filePath, 1595.f, 390.f, 1.2f);
}
//------------------------------------------------------------------------------------------------------------
void Map::createObstacle(std::string& filePath, float x, float y, float scaleX, float scaleY)
{
    sf::Sprite obstacle(AssetManager::GetTexture(filePath));
    obstacle.setPosition((x * window_size_.x) / 1920, (y * window_size_.y) / 1080);
    obstacle.scale((scaleX * window_size_.x) / 1920, (scaleY * window_size_.y) / 1080);
    obstacles_.push_back(obstacle);
}
//------------------------------------------------------------------------------------------------------------
void Map::checkInteractions(sf::Sprite& spriteObject, bool& onGround, bool* isJump)
{
    for (int i = 0; i < obstacles_.size(); ++i)
    {
        if (spriteObject.getGlobalBounds().intersects(obstacles_[i].getGlobalBounds()))
        {
            sf::FloatRect intersection;
            spriteObject.getGlobalBounds().intersects(obstacles_[i].getGlobalBounds(), intersection);

            if (intersection.width > intersection.height)
            {
                if (spriteObject.getPosition().y <= obstacles_[i].getPosition().y)
                {
                    if (spriteObject.getPosition().x >= obstacles_[i].getPosition().x && spriteObject.getPosition().x <= obstacles_[i].getPosition().x + obstacles_[i].getGlobalBounds().width)
                    {
                        spriteObject.setPosition(spriteObject.getPosition().x, obstacles_[i].getPosition().y - spriteObject.getGlobalBounds().height);
                        onGround = true;
                        current_obstacle_ = i;
                    }

                }
                else
                {
                    spriteObject.setPosition(spriteObject.getPosition().x, obstacles_[i].getPosition().y + obstacles_[i].getGlobalBounds().height);
                    *isJump = false;
                }
            }
            else
            {
                if (spriteObject.getPosition().x < obstacles_[i].getPosition().x)
                {
                    spriteObject.setPosition(obstacles_[i].getPosition().x - spriteObject.getGlobalBounds().width, spriteObject.getPosition().y);
                }
                else
                {
                    spriteObject.setPosition(obstacles_[i].getPosition().x + obstacles_[i].getGlobalBounds().width + spriteObject.getGlobalBounds().width, spriteObject.getPosition().y);
                }
            }
        }
    }

    if (current_obstacle_ != -1)
    {
        if (spriteObject.getPosition().x < obstacles_[current_obstacle_].getPosition().x || spriteObject.getPosition().x > obstacles_[current_obstacle_].getPosition().x + obstacles_[current_obstacle_].getGlobalBounds().width)
        {
            onGround = false;
            current_obstacle_ = -1;
        }
    }
}
//------------------------------------------------------------------------------------------------------------
void Map::draw(sf::RenderWindow& window)
{
    for (const auto& obstacle : obstacles_)
    {
        window.draw(obstacle);
    }
}
//------------------------------------------------------------------------------------------------------------