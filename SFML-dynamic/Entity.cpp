/**
* @file
* Entity.cpp
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
#include "Entity.h"
#include <cassert>

namespace GEX
{
	Entity::Entity(int points)
		: hitpoints_(points)
	{

	}
	void Entity::setVelocity(sf::Vector2f velocity)
	{
		velocity_ = velocity;
	}

	void Entity::setVelocity(float vx, float vy)
	{
		velocity_.x = vx;
		velocity_.y = vy;
	}

	sf::Vector2f Entity::getVelocity() const
	{
		return velocity_;
	}

	void Entity::accelerate(sf::Vector2f velocity)
	{
		velocity_ += velocity;
	}

	void Entity::accelerate(float vx, float vy)
	{
		velocity_.x += vx;
		velocity_.y += vy;
	}

	int Entity::getHitpoints() const
	{
		return hitpoints_;
	}

	void Entity::damage(int points)
	{
		assert(points > 0);
		points > hitpoints_ ? this->destroy() : hitpoints_ -= points;
	}

	void Entity::repair(int points)
	{
		assert(points > 0);
		hitpoints_ += points;
	}

	void Entity::destroy()
	{
		hitpoints_ = 0;
	}

	bool Entity::isDestroyed() const
	{
		return hitpoints_ == 0;
	}

	void Entity::remove()
	{
		destroy();
	}

	void Entity::updateCurrent(sf::Time dt, CommandQueue& Commands)
	{
		move(velocity_ * dt.asSeconds());
	}

}
