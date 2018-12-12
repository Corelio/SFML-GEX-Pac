/**
* @file
* Entity.cpp
* @author
* Marco Corsini Baccaro 2018
* @version 1.0
*
* @section DESCRIPTION
* Test #2 - Pacman (Dec, 12th)
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

	// Set the velocity to the entity - incoming parameter vector2F
	void Entity::setVelocity(sf::Vector2f velocity)
	{
		velocity_ = velocity;
	}

	// Set the velocity to the entity - incoming parameters x and y values
	void Entity::setVelocity(float vx, float vy)
	{
		velocity_.x = vx;
		velocity_.y = vy;
	}

	// get the entity velocity
	sf::Vector2f Entity::getVelocity() const
	{
		return velocity_;
	}

	// Adds the velocity to the entity - incoming parameter vector2F
	void Entity::accelerate(sf::Vector2f velocity)
	{
		velocity_ += velocity;
	}

	// Adds the velocity to the entity - incoming parameters x and y values
	void Entity::accelerate(float vx, float vy)
	{
		velocity_.x += vx;
		velocity_.y += vy;
	}

	// Get entity hit points (health)
	int Entity::getHitpoints() const
	{
		return hitpoints_;
	}

	// Remove hitpoints from the entity - remove health
	void Entity::damage(int points)
	{
		assert(points > 0);
		points > hitpoints_ ? this->destroy() : hitpoints_ -= points;
	}

	// Adds hitpoints to the entity - adds health
	void Entity::repair(int points)
	{
		assert(points > 0);
		hitpoints_ += points;
	}

	// Destroy the entity - hitpoints set to Zero
	void Entity::destroy()
	{
		hitpoints_ = 0;
	}

	// Check if the entity is destroyed
	bool Entity::isDestroyed() const
	{
		return hitpoints_ == 0;
	}

	// Destoy the entity
	void Entity::remove()
	{
		destroy();
	}

	// Update the entity - Movement
	void Entity::updateCurrent(sf::Time dt, CommandQueue& Commands)
	{
		// Hack to move the player only in one direction
		if (velocity_.x != 0 && velocity_.y != 0) {
			// Keeping the direction with greater module
			if (std::abs(velocity_.x) > std::abs(velocity_.y)) {
				velocity_.y = 0.f;
			}
			else {
				velocity_.x = 0.f;
			}
		}
		move(velocity_ * dt.asSeconds());
	}

}
