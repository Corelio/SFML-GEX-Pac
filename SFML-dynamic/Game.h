//
// Game Class
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "TextureManager.h"
#include "World.h"
#include "PlayerControl.h"
#include "CommandQueue.h"

#pragma once
namespace GEX
{
	class Game
	{
	public:
		Game();
		~Game();

		void run();

	private:
		void					processInputs();
		void					update(sf::Time deltaTime);
		void					render();
		void					updateStatistics(sf::Time deltaTime);

		//void					handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
		/* migrate to the world class
		void					drawBoundingBox();
		void					loadTextures();
		*/

	private:
		sf::RenderWindow		window_;
		World					world_;
		PlayerControl			player_;
		//sf::Sprite				player_; //World
		//sf::Texture				texture_;
		//GEX::TextureManager		textures_;
		sf::Vector2f			velocity_;
		sf::Text				statisticsText_;
		sf::Time				statisticsUpdateTime_;
		sf::Font				font_;
		unsigned int			statisticsNumFrames_;
		int						rotation_ = 0;
		sf::Vector2f			scale_;

		const sf::Time SPF = sf::seconds(1.0f / 60.0f); //Seconds per frame from 60fps
	};
}
