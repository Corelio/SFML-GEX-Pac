/**
* @file
* Animation2.cpp
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
#include "Animation2.h"


namespace GEX
{
	Animation2::Animation2(bool repeat)
		: repeat_(repeat)
		, currentFrame_(0)
		, duration_(sf::Time::Zero)
		, elapsedTime_(sf::Time::Zero)
	{

	}

	void Animation2::addFrame(Frame frame)
	{
		frames_.emplace_back(frame);
	}

	void Animation2::addFrameSet(std::vector<Frame> frames)
	{
		frames_ = frames;
	}

	void Animation2::setDuration(sf::Time time)
	{
		duration_ = time;
	}

	sf::Time Animation2::getDuration() const
	{
		return duration_;
	}
	void Animation2::setRepeating(bool repeat)
	{
		repeat_ = repeat;
	}

	bool Animation2::isRepeating() const
	{
		return repeat_;
	}

	void Animation2::restart()
	{
		currentFrame_ = 0;
	}

	bool Animation2::isFinished() const
	{
		return !repeat_ && currentFrame_ >= frames_.size();
	}

	Frame Animation2::getCurrentFrame() const
	{
		return frames_.at(currentFrame_ >= frames_.size() ?  frames_.size() -1 : currentFrame_) ;
	}

	Frame Animation2::update(sf::Time dt)
	{
		sf::Time timePerFrame = duration_ / static_cast<float>(frames_.size());
		elapsedTime_ += dt;

		while (elapsedTime_ >= timePerFrame && (currentFrame_ < frames_.size() || repeat_))
		{
			elapsedTime_ -= timePerFrame;
			currentFrame_++;
			if (repeat_)
			{
				currentFrame_ %= frames_.size();
			}
		}

		return getCurrentFrame();
	}

}
