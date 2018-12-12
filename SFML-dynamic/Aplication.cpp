/**
* @file
* Aplication.cpp
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
#include "Aplication.h"
#include "State.h"
#include "GameState.h"
#include "TitleState.h"
#include "MenuState.h"
#include "PauseState.h"
#include "GexState.h"
#include "GameOverState.h"
#include "FontManager.h"

//Update timeframe
const sf::Time Aplication::TimePerFrame = sf::seconds(1.0f / 60.0f);

Aplication::Aplication()
	: window_(sf::VideoMode(480, 480), "Pacman Test")
	, player_()
	, textures_()
	, music_()
	, sound_()
	, stateStack_(GEX::State::Context(window_, textures_, player_, music_, sound_))
	, statisticsText_()
	, statisticsUpdateTime_()
	, statisticsNumFrames_(0)
{
	window_.setKeyRepeatEnabled(false);

	GEX::FontManager::getInstance().load(GEX::FontID::Main, "Media/Sansation.ttf");

	textures_.load(GEX::TextureID::TitleScreen, "Media/Textures/TitleScreen.png");
	textures_.load(GEX::TextureID::Face, "Media/Textures/Face.png");

	statisticsText_.setFont(GEX::FontManager::getInstance().get(GEX::FontID::Main));
	statisticsText_.setPosition(5.0f, 5.0f);
	statisticsText_.setCharacterSize(12.0f);
	statisticsText_.setString("Frames / Second = \nTime / Update =");

	registerStates();
	stateStack_.pushState(GEX::StateID::Title);
}

//Run the application
void Aplication::run()
{
	//Creates clock to collect time 
	sf::Clock clock;

	//Start the delta time variable
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	//only run id the window is open
	while (window_.isOpen())
	{
		//add the delta time
		timeSinceLastUpdate += clock.restart();
		//If delta time is greater than the timeframe update
		while (timeSinceLastUpdate > TimePerFrame)
		{
			//remove delta time
			timeSinceLastUpdate -= TimePerFrame;

			//process all keyboard inputs
			processInputs();
			//update all states
			update(TimePerFrame);
			
			//If there is no states -> end the game
			if (stateStack_.isEmpty())
			{
				window_.close();
			}
		}
		
		//Update the frames per second statistics
		updateStatistics(timeSinceLastUpdate);

		//render the window
		render();
	}
}

//Process all keyboard inputs
void Aplication::processInputs()
{
	sf::Event event;
	while (window_.pollEvent(event))
	{
		stateStack_.handleEvent(event);
		if (event.type == sf::Event::Closed)
		{
			window_.close();
		}
	}
}

//Ask the states in the state stack to update
void Aplication::update(sf::Time dt)
{
	stateStack_.update(dt);
}

//render the window
void Aplication::render()
{
	window_.clear();
	stateStack_.draw();

	window_.setView(window_.getDefaultView());
	window_.draw(statisticsText_);
	window_.display();
}

//Update the statistics for the framerate
void Aplication::updateStatistics(sf::Time dt)
{
	statisticsUpdateTime_ += dt;
	statisticsNumFrames_++;

	if (statisticsUpdateTime_ > sf::seconds(1))
	{
		statisticsText_.setString(
			"Frames / Second = " + std::to_string(statisticsNumFrames_) + "\n" +
			"Time / Update   = " + std::to_string(statisticsUpdateTime_.asMicroseconds() / statisticsNumFrames_) + "ms"
		);
		statisticsNumFrames_ = 0;
		statisticsUpdateTime_ -= sf::seconds(1);
	}
}

//register all states for the application
void Aplication::registerStates()
{
	stateStack_.registerState<TitleState>(GEX::StateID::Title);
	stateStack_.registerState<MenuState>(GEX::StateID::Menu);
	stateStack_.registerState<GameState>(GEX::StateID::Game);
	stateStack_.registerState<PauseState>(GEX::StateID::Pause);
	stateStack_.registerState<GexState>(GEX::StateID::Gex);
	stateStack_.registerState<GameOverState>(GEX::StateID::GameOver);

}
