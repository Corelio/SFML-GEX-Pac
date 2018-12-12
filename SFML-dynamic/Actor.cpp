/**
* @file
* Actor.cpp
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
#include "Actor.h"
#include "TextureManager.h"
#include "JsonFrameParser.h"
#include "TextureManager.h"
#include "Utility.h"
#include "DataTables.h"
#include "TextNode.h"
#include <memory>

namespace GEX
{
	namespace
	{
		const std::map<ActorType, ActorData> TABLE = initializeActorData();
	}

	Actor::Actor(ActorType type, const GEX::TextureManager& textures)
		: Entity(100)
		, type_(type)
		, textures_(textures)
		, state_(State::Walk)
		, sprite_(textures.get(TABLE.at(type).texture))
		, direction_(Direction::Right)
		, travelDistance_(0.f)
		, directionIndex_(0)
		, power_(false)
		, elapsedPowerTime_(sf::seconds(4.f))
		, shouldBeAffraid_(false)
		, retreatElapsed_(sf::Time::Zero)

	{	  
		//Load animations map
		for (auto a : TABLE.at(type).animations)
		{
			animations_[a.first] = a.second;
		}

		//Start with an empty sprite to prevent the flash of the whole texture
		sprite_.setTextureRect(sf::IntRect());

		//center the sprite
		centerOrigin(sprite_);
	}

	//Returns the Actor category
	unsigned int Actor::getCategory() const
	{
		switch (type_)
		{
		case ActorType::Ghost:
			return Category::Ghost;
		case ActorType::Pacman:
			return Category::Pacman;
		case ActorType::Cherry:
			return Category::Cherry;
		case ActorType::Power:
			return Category::Power;
		}
	}

	//Get the bounding box for collisions
	sf::FloatRect Actor::getBoundingBox() const
	{
		auto box = getWorldTransform().transformRect(sprite_.getGlobalBounds());
		box.width -= 30; // tighten up bounding box for more realistic collisions
		box.left += 15;
		return box;
	}

	//Get the Actor max speed
	float Actor::getMaxSpeed() const
	{
		return TABLE.at(type_).speed;
	}

	//Acelerate the Actor
	void Actor::accelerate(sf::Vector2f velocity)
	{
		Entity::accelerate(velocity);
	}


	//Set the actor state
	void Actor::setState(State state)
	{
		state_ = state;
		animations_[state_].restart();
	}

	//Get the actor state
	Actor::State Actor::getState() const
	{
		return state_;
	}

	//Get the actor direction - used to rotate the player
	Actor::Direction Actor::getDirection() const
	{
		return direction_;
	}

	//Update Actor state
	void Actor::updateStates(sf::Time dt)
	{
		//If actor hitppoints is zero -> start death animation
		if (isDestroyed()) {
			state_ = State::Dead;
		}

		//Check if the actor should be affraid
		if (shouldBeAffraid_) {
			if (state_ != State::RetreatStart && state_ != State::RetreatEnd) {
				state_ = State::RetreatStart;
				retreatElapsed_ = sf::Time::Zero;
			}
			else {
				retreatElapsed_ += dt;
			}

			if (retreatElapsed_ >= sf::seconds(3)) {
				state_ = State::RetreatEnd;
			}
		}
		else {
			//Update ghost "eyes"/Direction
			if (state_ != State::Dead && type_ == ActorType::Ghost)
			{
				if (getVelocity().y != 0)
				{
					if (getVelocity().y < 0)
					{
						state_ = State::WalkUp;
					}
					else
					{
						state_ = State::WalkDown;
					}
				}
				else {
					if (getVelocity().x < 0)
					{
						state_ = State::WalkLeft;
					}
					else
					{
						state_ = State::WalkRight;
					}
				}
			}
		}
	}

	//Update Actor
	void Actor::updateCurrent(sf::Time dt, GEX::CommandQueue & commands)
	{
		//Update States
		updateStates(dt);

		//Get the animation
		auto rec = animations_.at(state_).update(dt);

		//Check direction to flip the character (only if you are Pacman)
		if (type_ == ActorType::Pacman)
		{
			if (direction_ == Direction::Left && getVelocity().x > 0)
				direction_ = Direction::Right;
			if (direction_ == Direction::Right && getVelocity().x < 0)
				direction_ = Direction::Left;

			// flip image left right
			if (direction_ == Direction::Left)
				rec = flip(rec);
		}

		//Set the sprite position over the texture
		sprite_.setTextureRect(rec);

		// Change the color of the special cherry
		if (type_ == ActorType::Power) {
				sprite_.setColor(sf::Color::Magenta);
		}

		//Center the sprite
		centerOrigin(sprite_);

		//Update the entity, only if the Actor is not dead
		if (state_ != State::Dead) {// dont move it while dying
			Entity::updateCurrent(dt, commands);
		}

		//Pacman Power
		if (power_)
		{
			if (elapsedPowerTime_ <= sf::Time::Zero)
			{
				power_ = false;
			}
			else
			{
				elapsedPowerTime_ -= dt;
			}
		}
	}

	//Draw the Actor
	void Actor::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
	{
		target.draw(sprite_, states);
	}

	//Check if the actor is read to be removed 
	bool Actor::isMarkedForRemoval() const
	{
		//Actor must be destroied and the dead animation must finish
		return isDestroyed() && state_ == State::Dead && animations_[state_].isFinished(); 
	}

	bool Actor::hasPower() const
	{
		return power_;
	}

	void Actor::addPower()
	{
		power_ = true;
		elapsedPowerTime_ = sf::seconds(10.f);
	}

	void Actor::removePower()
	{
		power_ = false;
		elapsedPowerTime_ = sf::Time::Zero;
	}

	void Actor::shouldBeAffraid(bool beAffraid)
	{
		shouldBeAffraid_ = beAffraid;
	}


}
