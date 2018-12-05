/**
* @file
* MenuState.cpp
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
#include "MenuState.h"
#include "Utility.h"
#include "FontManager.h"
#include "ResourceIdentifier.h"

MenuState::MenuState(GEX::StateStack& stack, Context context)
	: GEX::State(stack, context)
	, options_()
	, optionsIndex_(0)
{
	sf::Font& font = GEX::FontManager::getInstance().get(GEX::FontID::Main);
	backgroundSprite_.setTexture(context.textures_->get(GEX::TextureID::TitleScreen));

	//set up menu
	//Play Option
	sf::Text playOption;
	playOption.setFont(font);
	playOption.setString("Play");
	centerOrigin(playOption);
	playOption.setPosition(context.window_->getView().getSize() / 2.f);
	
	//Exit Option
	sf::Text exitOption;
	exitOption.setFont(font);
	exitOption.setString("Exit");
	centerOrigin(exitOption);
	exitOption.setPosition(playOption.getPosition() + sf::Vector2f(0.f, 30.f));
	
	options_.push_back(playOption);
	options_.push_back(exitOption);

	updateOptionText();

	context.music_->play(GEX::MusicID::MenuTheme);
}

void MenuState::draw()
{
	auto& window = *getContext().window_;

	window.setView(window.getDefaultView());
	window.draw(backgroundSprite_);

	for (const sf::Text& text : options_)
	{
		window.draw(text);
	}
	
}

//All other states bellow this could be updated
bool MenuState::update(sf::Time dt)
{
	return true;
}

//handle key press event -> start game or exit
bool MenuState::handleEvent(const sf::Event & event)
{
	if (event.type != sf::Event::KeyPressed)
	{
		return true;
	}
	if (event.key.code == sf::Keyboard::Return)
	{
		if (optionsIndex_ == Play)
		{
			requestStackPop();
			requestStackPush(GEX::StateID::Game);
		}
		else if (optionsIndex_ == Exit)
		{
			requestStackPop();
		}
	}
	else if (event.key.code == sf::Keyboard::Up)
	{
		if (optionsIndex_ > 0)
		{
			optionsIndex_--;
		}
		else
		{
			optionsIndex_ = options_.size() - 1;
		}
		updateOptionText();
	}
	else if (event.key.code == sf::Keyboard::Down)
	{
		if (optionsIndex_ < options_.size() - 1)
		{
			optionsIndex_++;
		}
		else
		{
			optionsIndex_ = 0;
		}
		updateOptionText();
	}

	return true;
}

//Change color of the menu options
void MenuState::updateOptionText()
{
	if (options_.empty())
	{
		return;
	}

	for (sf::Text& text : options_)
	{
		text.setFillColor(sf::Color::White);
		text.setOutlineColor(sf::Color::Black);
	}

	options_.at(optionsIndex_).setFillColor(sf::Color::Magenta);
}
