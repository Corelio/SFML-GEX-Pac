/**
* @file
* World.h
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
#pragma once
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "SceneNode.h"
#include "SpriteNode.h"
#include "TextureManager.h"
#include <memory>
#include "CommandQueue.h"
#include "SoundPlayer.h"

namespace sf  //Forward declaration - This class does not need to know about this class
{
	class RenderTarget;
}

namespace GEX
{
	class CommandQueue;
	class Actor;
	class World
	{
	public:
		explicit					World(sf::RenderTarget& window, SoundPlayer& sounds);

		void						update(sf::Time dt, CommandQueue& commands);
		void						draw();

		CommandQueue&				getCommandQueue();
		bool						hasAlivePlayer();

	private:
		void						loadTextures();
		void						buildScene();
		void						adaptPlayerPosition();

		void						adaptGhostPosition();
		void						updateTexts();

		sf::FloatRect				getViewBounds() const;
		sf::FloatRect				getBattlefieldBounds() const;

		void						handleCollision();

		void						updateSound();

		void						spawnPlayer();

		void						chasePlayer();

		void						informPowerToGhost();

		sf::Vector2f				randowPositionInsideWorldBounds();

		void						checkScore();

	private:
		enum Layer
		{
			Background = 0,
			LowerAir,
			UpperAir,
			LayerCount
		};

	private:
		const float					BORDER_DISTANCE = 40.f;

		sf::RenderTarget&			target_;
		sf::RenderTexture			sceneTexture_;
		sf::View					worldView_;
		TextureManager				textures_;
		SoundPlayer&				sounds_;

		SceneNode					sceneGraph_;
		std::vector<SceneNode*>		sceneLayers_;

		sf::FloatRect				worldBounds_;
		sf::Vector2f				spawnPosition_;
		sf::Vector2f				pacmanSpawnPosition_;
		float						scrollSpeed_;
		int							counter_;
		int							orientation_;
		mutable Actor*				player_;
		Actor*						ghost_;
		SpriteNode*					background_;

		CommandQueue				commandQueue_;

		int							lives_;
		int							score_;
		sf::Text					scoreText_;
		sf::Text					livesText_;

		float						distanceToChase_;
		int							lastCheckedScore_;

	};

}