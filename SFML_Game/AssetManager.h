#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <assert.h>
#include<map>

//------------------------------------------------------------------------------------------------------------
class AssetManager
{
public:
    AssetManager();

    static sf::Texture& GetTexture(const std::string& filename);
    static sf::SoundBuffer& GetSoundBuffer(const std::string& filename);
    static sf::Font& GetFont(const std::string& filename);

private:
    template<typename T>
    T& getResource(std::map<std::string, T>& resourceMap, const std::string& filename);

    std::map<std::string, sf::Texture> textures_;
    std::map<std::string, sf::SoundBuffer> sound_buffer_;
    std::map<std::string, sf::Font> fonts_;

    static AssetManager* sInstance_;
};
//------------------------------------------------------------------------------------------------------------