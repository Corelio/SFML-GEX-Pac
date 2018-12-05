/**
* @file
* Animation.cpp
* @author
* Marco Corsini Baccaro 2018
* @version 1.0
*
* @section DESCRIPTION
* Test #1 - Pacman (Nov, 23th)
*
* @section LICENSE
*
* Copyright 2018
* Permission to use, copy, modify, and/or distribute this software for
* any purpose with or without fee is hereby granted, provided that the
* above copyright notice and this permission notice appear in all copies.
*
* THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
* WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
* ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
* WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
* ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
* OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*
* @section Academic Integrity
* I certify that this work is solely my own and complies with
* NBCC Academic Integrity Policy (policy 1111)
*/
#include "Animation.h"

namespace GEX
{
	Animation::Animation()
		: sprite_()
		, frameSize_()
		, numberOfFrames_(0)
		, currentFrame_(0)
		, duration_(sf::Time::Zero)
		, elapsedTime_(sf::Time::Zero)
		, repeat_(false)
	{
	}

	Animation::Animation(const sf::Texture & texture)
		: sprite_(texture)
		, frameSize_()
		, numberOfFrames_(0)
		, currentFrame_(0)
		, duration_(sf::Time::Zero)
		, elapsedTime_(sf::Time::Zero)
		, repeat_(false)
	{
		sprite_.setTextureRect(sf::IntRect());
	}

	void Animation::setTexture(const sf::Texture& texture)
	{
		sprite_.setTexture(texture);
	}

	const sf::Texture * Animation::getTexture() const
	{
		return sprite_.getTexture();
	}

	void Animation::setFrameSize(sf::Vector2i framesize)
	{
		frameSize_ = framesize;
	}

	sf::Vector2i Animation::getFrameSize() const
	{
		return frameSize_;
	}

	void Animation::setNumFrames(std::size_t numFrames)
	{
		numberOfFrames_ = numFrames;
	}

	std::size_t Animation::getNumFrames() const
	{
		return numberOfFrames_;
	}

	void Animation::setDuration(sf::Time time)
	{
		duration_ = time;
	}

	sf::Time Animation::getDuration() const
	{
		return duration_;
	}

	void Animation::setRepeating(bool repeat)
	{
		repeat_ = repeat;
	}

	bool Animation::isRepeating() const
	{
		return repeat_;
	}

	void Animation::restart()
	{
		currentFrame_ = 0;
	}

	bool Animation::isFinished() const
	{
		return currentFrame_ >= numberOfFrames_;
	}

	sf::FloatRect Animation::getLocalBounds() const
	{
		return sf::FloatRect(getOrigin(), static_cast<sf::Vector2f>(getFrameSize()));
	}

	sf::FloatRect Animation::getGlobalBounds() const
	{
		return getTransform().transformRect(getLocalBounds());
	}

	void Animation::update(sf::Time dt)
	{
		sf::Time timePerFrame = duration_ / static_cast<float>(numberOfFrames_);
		elapsedTime_ += dt;

		sf::Vector2i textureBonds(sprite_.getTexture()->getSize());
		sf::IntRect	textureRect = sprite_.getTextureRect();

		if (currentFrame_ == 0) {
			textureRect = sf::IntRect(0, 0, frameSize_.x, frameSize_.y);
		}

		while (elapsedTime_ >= timePerFrame && (currentFrame_ <= numberOfFrames_ || repeat_))
		{
			// move to the next frame
			textureRect.left += textureRect.width;

			//go to the next row, if needed
			if (textureRect.left + textureRect.width > textureBonds.x)
			{
				textureRect.left = 0;
				textureRect.top += textureRect.height;
			}

			elapsedTime_ -= timePerFrame;

			if (repeat_)
			{
				currentFrame_ = (currentFrame_ + 1) % numberOfFrames_;
				if (currentFrame_ == 0) {
					textureRect = sf::IntRect(0, 0, frameSize_.x, frameSize_.y);
				}
			}
			else
			{
				currentFrame_++;
			}
		}

		sprite_.setTextureRect(textureRect);
	}

	void Animation::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		target.draw(sprite_, states);
	}

}