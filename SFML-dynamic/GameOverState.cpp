/**
* @file
* GameOverState.cpp
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
#include "GameOverState.h"
#include "Utility.h"
#include "FontManager.h"
#include "PlayerControl.h"


GameOverState::GameOverState(GEX::StateStack& stack, Context context)
	: State(stack, context)
	, gameOverText_()
	, elapsedTime_(sf::Time::Zero)
	, waitingTime_(2)
{
	sf::Vector2f windowSize(context.window_->getSize());

	gameOverText_.setFont(GEX::FontManager::getInstance().get(GEX::FontID::Main));
	if (context.player_->getMissionStatus() == GEX::MissionStatus::MissionFailure)
	{
		gameOverText_.setString("Game Over");
	}
	else
	{
		gameOverText_.setString("Mission Success");
	}
	gameOverText_.setCharacterSize(70);
	centerOrigin(gameOverText_);
	gameOverText_.setPosition(windowSize.x * .5f, windowSize.y * .4f);

}

void GameOverState::draw()
{
	sf::RenderWindow& window = *getContext().window_;
	window.setView(window.getDefaultView());

	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(0, 0, 0, 50));
	backgroundShape.setSize(window.getView().getSize());

	window.draw(backgroundShape);
	window.draw(gameOverText_);
}

bool GameOverState::update(sf::Time dt)
{
	elapsedTime_ += dt;
	if (elapsedTime_ > sf::seconds(waitingTime_))
	{
		requestStackClear();
		requestStackPush(GEX::StateID::Menu);
	}
	return false;
}

bool GameOverState::handleEvent(const sf::Event & event)
{
	return false;
}

