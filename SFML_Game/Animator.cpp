#include "Animator.h"

// Animator
//------------------------------------------------------------------------------------------------------------
Animator::Animator(sf::Sprite& sprite)
	: CurrentAnimation_(nullptr), sprite_(sprite), current_frame_(0), end_anim_(false)
{
}
//------------------------------------------------------------------------------------------------------------
Animator::Animation& Animator::createAnimation(std::string const& name,
	std::string const& textureName, sf::Time const& duration, bool loop)
{
	Animations_.emplace_back(name, textureName, duration, loop);

	if (CurrentAnimation_ == nullptr)
	{
		switchAnimation(&Animations_.back());
	}

	return Animations_.back();
}
//------------------------------------------------------------------------------------------------------------
void Animator::switchAnimation(Animator::Animation* animation)
{
	if (animation != nullptr)
	{
		sprite_.setTexture(AssetManager::GetTexture(animation->texture_name));
	}

	CurrentAnimation_ = animation;
	current_time_ = sf::Time::Zero;
}
//------------------------------------------------------------------------------------------------------------
bool Animator::switchAnimation(std::string const& name)
{
	auto animation = findAnimation(name);
	if (animation != nullptr)
	{
		switchAnimation(animation);
		return true;
	}
	return false;
}
//------------------------------------------------------------------------------------------------------------
Animator::Animation* Animator::findAnimation(std::string const& name)
{
	for (auto it = Animations_.begin(); it != Animations_.end(); it++)
	{
		if (it->name == name)
		{
			return &*it;
		}
	}
	return nullptr;
}
//------------------------------------------------------------------------------------------------------------
void Animator::restart()
{
	current_time_ = sf::Time::Zero;
	end_anim_ = false;
}
//------------------------------------------------------------------------------------------------------------
void Animator::update(sf::Time const& dt)
{
	if (CurrentAnimation_ == nullptr)
	{
		return;
	}

	current_time_ += dt;

	float scaledTime = (current_time_.asSeconds() / CurrentAnimation_->duration.asSeconds());
	auto numFrames = static_cast<int>(CurrentAnimation_->frames.size());
	current_frame_ = static_cast<int>(scaledTime * numFrames);

	if (CurrentAnimation_->looping)
	{
		current_frame_ %= numFrames;
	}
	else
	{
		if (current_frame_ >= numFrames)
		{
			current_frame_ = numFrames - 1;
			end_anim_ = true;
		}
	}

	sprite_.setTextureRect(CurrentAnimation_->frames[current_frame_]);
}
//------------------------------------------------------------------------------------------------------------
std::string Animator::getCurrentAnimationName() const
{
	if (CurrentAnimation_ != nullptr)
	{
		return CurrentAnimation_->name;
	}
	return "";
}
//------------------------------------------------------------------------------------------------------------
bool Animator::getEndAnim() const
{
	return end_anim_;
}
//------------------------------------------------------------------------------------------------------------
const int Animator::getCurrentFrame() const
{
	return current_frame_;
}
//------------------------------------------------------------------------------------------------------------