#include "AssetManager.h"

// AssetManager
//------------------------------------------------------------------------------------------------------------
AssetManager* AssetManager::sInstance_ = nullptr;
//------------------------------------------------------------------------------------------------------------
AssetManager::AssetManager()
{
    assert(sInstance_ == nullptr);
    sInstance_ = this;
}
//------------------------------------------------------------------------------------------------------------
sf::Texture& AssetManager::GetTexture(const std::string& filename)
{
    return sInstance_->getResource(sInstance_->textures_, filename);
}
//------------------------------------------------------------------------------------------------------------
sf::SoundBuffer& AssetManager::GetSoundBuffer(const std::string& filename)
{
    return sInstance_->getResource(sInstance_->sound_buffer_, filename);
}
//------------------------------------------------------------------------------------------------------------
sf::Font& AssetManager::GetFont(const std::string& filename)
{
    return sInstance_->getResource(sInstance_->fonts_, filename);
}
//------------------------------------------------------------------------------------------------------------
template<typename T>
T& AssetManager::getResource(std::map<std::string, T>& resourceMap, const std::string& filename)
{
    auto pair_found = resourceMap.find(filename);
    if (pair_found != resourceMap.end())
    {
        return pair_found->second;
    }
    else
    {
        auto& resource = resourceMap[filename];
        resource.loadFromFile(filename);
        return resource;
    }
}
//------------------------------------------------------------------------------------------------------------