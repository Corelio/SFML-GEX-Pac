/**
* @file
* World.cpp
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
#include "World.h"
#include <set>
#include <memory>
#include <SFML/Graphics/RenderTarget.hpp>
#include "Utility.h"
#include "Actor.h"
#include "FontManager.h"
#include <iostream>

namespace GEX
{

	World::World(sf::RenderTarget& outputTarget, SoundPlayer& sounds)
		: target_(outputTarget)
		, sounds_(sounds)
		, worldView_(outputTarget.getDefaultView())
		, textures_()
		, sceneGraph_()
		, sceneLayers_()
		, worldBounds_(0.f, 0.f, worldView_.getSize().x, worldView_.getSize().y)
		, spawnPosition_(worldView_.getSize().x/2.f, worldBounds_.height - (worldView_.getSize().y / 2.f))
		, scrollSpeed_(0.f)
		, counter_(1)
		, orientation_(1)
		, commandQueue_()
		, lives_(3)
		, score_(0)
		, lastCheckedScore_(0)
		, scoreText_()
		, livesText_()
		, pacmanSpawnPosition_(40.f, worldBounds_.height / 2.f)
		, distanceToChase_(worldBounds_.height * 0.3f)
	{

		sceneTexture_.create(target_.getSize().x, target_.getSize().y);

		//Load all textures
		loadTextures();

		//prepare the view

		worldView_.setCenter(spawnPosition_);

		// Score and lives text
		scoreText_.setFont(GEX::FontManager::getInstance().get(GEX::FontID::Main));
		scoreText_.setPosition(5.0f, 40.0f);
		scoreText_.setCharacterSize(16.0f);
		scoreText_.setString("Score: " + std::to_string(score_));

		livesText_.setFont(GEX::FontManager::getInstance().get(GEX::FontID::Main));
		livesText_.setPosition(5.0f, 60.0f);
		livesText_.setCharacterSize(16.0f);
		livesText_.setString("Lives: " + std::to_string(lives_));

		//Build the world
		buildScene();
	}

	void World::update(sf::Time dt, CommandQueue& commands)
	{
		//Reset the player velocity
		player_->setVelocity(0.f, 0.f);

		// OK That os a work around, but is the easy way to do that
		// Pass the pacman's power state of the ghost 
		informPowerToGhost();

		// run all commands in the command queue
		while (!commandQueue_.isEmpty())
		{
			sceneGraph_.onCommand(commandQueue_.pop(), dt);
		}

		//Handleling collisions
		handleCollision();

		//remove all wrecks from the scene graph
		sceneGraph_.removeWrecks();

		//All scene nodes should be updated
		sceneGraph_.update(dt, commands);

		//Check if player is destroyed
		if (!player_->isDestroyed())
		{
			//If its is not run the player position adapter
			adaptPlayerPosition();
			//Chase player if in range
			if (ghost_->getHitpoints() > 0)
			{
				chasePlayer();
			}
		}

		//Adapt the Ghost position
		if (ghost_->getHitpoints() > 0)
		{
			adaptGhostPosition();
		}
		

		//check if there are any enemy inside of the battlefield and spawn it
		updateSound();

		//Update Texts
		updateTexts();

		//Update lives if score is % 1000
		checkScore();

	}

	//Adapt/correct/change actor position
	void World::adaptPlayerPosition()
	{
		//Don't allow the Actor to leave the screen
		sf::FloatRect viewBounds(worldView_.getCenter()-worldView_.getSize() / 2.f, worldView_.getSize());

		sf::Vector2f position = player_->getPosition();
		position.x = std::max(position.x, viewBounds.left + BORDER_DISTANCE);
		position.x = std::min(position.x, viewBounds.left + viewBounds.width - BORDER_DISTANCE);

		position.y = std::max(position.y, viewBounds.top + BORDER_DISTANCE);
		position.y = std::min(position.y, viewBounds.top + viewBounds.height - BORDER_DISTANCE);

		player_->setPosition(position);

		//Rotate the actor -> being lazy/saving time
		sf::Vector2f velocity = player_->getVelocity();
		Actor::Direction direction = player_->getDirection();
		player_->setRotation(0);
		//Check player direction too - to rotate the image to the right direction
		if (velocity.y < 0 && direction == Actor::Direction::Left)
		{
			player_->setRotation(90);
		}
		else if (velocity.y < 0 && direction == Actor::Direction::Right)
		{
			player_->setRotation(270);
		}
		if (velocity.y > 0 && direction == Actor::Direction::Left)
		{
			player_->setRotation(270);
		}
		else if (velocity.y > 0 && direction == Actor::Direction::Right)
		{
			player_->setRotation(90);
		}
	}

	//Change ghost moviment if reachs the end of the battlefield
	//Change the state of the ghost too
	// Being lazy again to save time
	void World::adaptGhostPosition()
	{
		sf::FloatRect viewBounds(worldView_.getCenter() - worldView_.getSize() / 2.f, worldView_.getSize());

		sf::Vector2f position = ghost_->getPosition();

		//If ghost is close to the top or bottom border
		//MOve it back inside
		position.x = std::max(position.x, viewBounds.left + BORDER_DISTANCE);
		position.x = std::min(position.x, viewBounds.left + viewBounds.width - BORDER_DISTANCE);

		position.y = std::max(position.y, viewBounds.top + BORDER_DISTANCE);
		position.y = std::min(position.y, viewBounds.top + viewBounds.height - BORDER_DISTANCE);

		ghost_->setPosition(position);

		//Switch Y velocity direction
		//Switch state -> ghost eyes ;)
		if (position.y <= viewBounds.top + BORDER_DISTANCE || position.y >= viewBounds.top + viewBounds.height - BORDER_DISTANCE)
		{
			ghost_->setVelocity(ghost_->getVelocity().x, ghost_->getVelocity().y * -1.f);
		}
		if (position.x <= viewBounds.left + BORDER_DISTANCE || position.x >= viewBounds.left + viewBounds.width - BORDER_DISTANCE)
		{
			ghost_->setVelocity(ghost_->getVelocity().x * -1.f, ghost_->getVelocity().y);
		}
	}

	//Update the score and live text
	void World::updateTexts()
	{
		//Update texts
		scoreText_.setString("Score: " + std::to_string(score_));
		livesText_.setString("Lives: " + std::to_string(lives_));

		//change live color is is the last live
		livesText_.setFillColor(sf::Color::White);
		if (lives_ == 1)
		{
			livesText_.setFillColor(sf::Color::Red);
		}
	}

	//Get the view bounds
	sf::FloatRect World::getViewBounds() const
	{
		return sf::FloatRect(worldView_.getCenter() - worldView_.getSize() / 2.f, worldView_.getSize());
	}

	//Get the active game area - Battle Field
	sf::FloatRect World::getBattlefieldBounds() const
	{
		sf::FloatRect bounds = getViewBounds();
		bounds.top -= 100.f;
		bounds.height += 100.f;
		return bounds;
	}

	//Check for matching collisions and order it if need
	bool matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2)
	{
		unsigned int category1 = colliders.first->getCategory();
		unsigned int category2 = colliders.second->getCategory();

		//Make sure first pair entry has category type1 and seconde has type2
		if (type1 & category1 && type2 & category2)
		{
			return true;
		}
		else if(type1 & category2 && type2 & category1)
		{
			std::swap(colliders.first, colliders.second);
			return true;
		}
		
		return false;
	
	}

	//Handle all actor collisions
	//Only two possible collisions
	//Pacman / Ghost -> pacman dies
	//Pacman / Cherry -> pacman gets 200 points and respawn at start position
	void World::handleCollision()
	{
		// Build a list of collinding Pairs of SceneNode
		std::set<SceneNode::Pair> collisionPairs;

		sceneGraph_.checkSceneCollision(sceneGraph_, collisionPairs);

		for (auto collindingPair : collisionPairs)
		{
			//Pacman - Cherry collision
			if (matchesCategories(collindingPair, Category::Type::Pacman, Category::Type::Cherry))
			{
				auto& Pacman = static_cast<Actor&>(*collindingPair.first);
				Pacman.setPosition(pacmanSpawnPosition_);
				score_ += 200;
				
			}
			//Pacman Ghost collision
			else if (matchesCategories(collindingPair, Category::Type::Pacman, Category::Type::Ghost))
			{
				auto& Pacman = static_cast<Actor&>(*collindingPair.first);
				auto& Ghost = static_cast<Actor&>(*collindingPair.second);

				if (Pacman.hasPower()) {
					Ghost.setPosition(randowPositionInsideWorldBounds());
					Pacman.removePower();
					score_ += 200;
				}
				else {
					Pacman.destroy();
				}

				
			}
			//Pacman Power collision
			else if (matchesCategories(collindingPair, Category::Type::Pacman, Category::Type::Power))
			{
				auto& Pacman = static_cast<Actor&>(*collindingPair.first);
				auto& Power  = static_cast<Actor&>(*collindingPair.second);

				Pacman.addPower();
				score_ += 200;
				Power.setPosition(randowPositionInsideWorldBounds());
			}
		}
	}

	//Update sounds - remove those that are stopped
	void World::updateSound()
	{
		sounds_.setListenerPosition(player_->getWorldPosition());
		sounds_.removeStoppedSounds();
	}

	//Spaw a new player
	void World::spawnPlayer()
	{
		std::unique_ptr<Actor> Pac(new Actor(ActorType::Pacman, textures_));
		Pac->setPosition(randowPositionInsideWorldBounds());
		Pac->setState(Actor::State::Walk); //Set start state
		player_ = Pac.get();
		sceneLayers_[UpperAir]->attachChild(std::move(Pac));
	}

	// Ghost chase pacman
	void World::chasePlayer()
	{
		// Get the distance between player and ghost
		auto d = distance(*player_, *ghost_);

		// If the pac is close tho the ghost, start chase
		if (d < distanceToChase_) {
			// get the velocity vector to move the ghost close to the pac
			sf::Vector2f velocityTemp = unitVector(player_->getWorldPosition() - ghost_->getWorldPosition())  * (player_->hasPower() ? -1.f : 1.f) * 100.f;	
			// We dont want diagonal moviments, so we will keep the orientation with greater value
			if (std::abs(velocityTemp.x) > std::abs(velocityTemp.y)) {
				velocityTemp.y = 0;
			}
			else {
				velocityTemp.x = 0;
			}
			// Assign the velocity to the ghost
			ghost_->setVelocity(velocityTemp);
		}
	}

	// Tell the ghost that the pacman has power -> Ghost should be affraid
	void World::informPowerToGhost()
	{
		ghost_->shouldBeAffraid(player_->hasPower());
	}

	// Generate a randow position inside the world bounds
	sf::Vector2f World::randowPositionInsideWorldBounds()
	{
		sf::FloatRect viewBounds(worldView_.getCenter() - worldView_.getSize() / 2.f, worldView_.getSize());
		int x = 0;
		while (x <= viewBounds.left + BORDER_DISTANCE) {
			x = randomInt(viewBounds.left + viewBounds.width - BORDER_DISTANCE);
		}
		int y = 0;
		while (y <= viewBounds.top + BORDER_DISTANCE) {
			y = randomInt(viewBounds.top + viewBounds.height - BORDER_DISTANCE);
		}

		return sf::Vector2f(x, y);

	}

	void World::checkScore()
	{
		//EXTRA - each 1000 points get a life
		if (score_ != lastCheckedScore_ && score_ % 1000 == 0)
		{
			lives_++;
			lastCheckedScore_ = score_;
		}
	}

	//Draw the world
	void World::draw()
	{
			target_.setView(worldView_);
			target_.draw(sceneGraph_);
			target_.draw(scoreText_);
			target_.draw(livesText_);
	}

	//Get the comman queue
	CommandQueue & World::getCommandQueue()
	{
		return commandQueue_;
	}

	/*
	Check if the player is dead 
	if player is dead, lose one live and respawn the player
	if player does not have any lives left, end the game
	*/
	bool World::hasAlivePlayer()
	{
		// Doing a lazy verification and restarting the player -> being lazy 
		// This could be improved

		if (player_->isMarkedForRemoval())
		{
			//Check for lives
			if (lives_ > 1) {
				//Decrease lives
				lives_--;
				//Spawn new player
				spawnPlayer();
			}
			else
			{
				//no more lives - end the game
				lives_--;
				return false;
			}
		}

		return true;
	}

	//Load textures
	void World::loadTextures()
	{
		textures_.load(GEX::TextureID::Jungle,		"Media/Textures/background.png");
		textures_.load(GEX::TextureID::Atlas,		"Media/Textures/Atlas.png");
		textures_.load(GEX::TextureID::PacmanAtlas,  "Media/Textures/pacManAtlas.png");
	}

	//Build the world
	void World::buildScene()
	{
		// Initialize layers
		for (int i = 0; i < LayerCount; ++i)
		{
			auto category = (i == UpperAir) ? Category::Type::AirSceneLayer : Category::Type::None;
			SceneNode::Ptr layer(new SceneNode(category));
			sceneLayers_.push_back(layer.get());
			sceneGraph_.attachChild(std::move(layer));
		}

		// background
		sf::Texture&		texture = textures_.get(TextureID::Jungle);
		sf::IntRect			textureRect(worldBounds_);
		texture.setRepeated(true);

		std::unique_ptr<SpriteNode>	backgroundSprite(new SpriteNode(texture, textureRect));

		backgroundSprite->setPosition(worldBounds_.left, worldBounds_.top);
		background_ = backgroundSprite.get();
		sceneLayers_[Background]->attachChild(std::move(backgroundSprite));


		// Adding Characters

		spawnPlayer();

		//Ghost
		std::unique_ptr<Actor> Ghost(new Actor(ActorType::Ghost, textures_));
		Ghost->setPosition(worldView_.getSize().x - worldView_.getSize().x * 0.3f, worldBounds_.top + 50.f);
		Ghost->setVelocity(0.f, 100.f);
		Ghost->setState(Actor::State::WalkDown); //Set start state
		ghost_ = Ghost.get();
		sceneLayers_[UpperAir]->attachChild(std::move(Ghost));
		
		//Cheryy
		std::unique_ptr<Actor> Cherry(new Actor(ActorType::Cherry, textures_));
		Cherry->setPosition(worldView_.getSize().x - worldView_.getSize().x * 0.1f, worldBounds_.height / 2.f);
		Cherry->setState(Actor::State::Idle); //Set start state
		sceneLayers_[UpperAir]->attachChild(std::move(Cherry));

		//Power Chery
		std::unique_ptr<Actor> Power(new Actor(ActorType::Power, textures_));
		Power->setPosition(worldView_.getSize().x - worldView_.getSize().x * 0.9f, worldBounds_.height - worldView_.getSize().y * 0.2f);
		Power->setState(Actor::State::Idle); //Set start state
		sceneLayers_[UpperAir]->attachChild(std::move(Power));
	}
}