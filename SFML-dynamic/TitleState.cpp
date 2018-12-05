/**
* @file
* TitleState.cpp
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
#include "TitleState.h"
#include "TextureManager.h"
#include "Utility.h"
#include "FontManager.h"

TitleState::TitleState(GEX::StateStack& stack, Context context)
	: GEX::State(stack, context)
	, backgroundSprite_()
	, text_()
	, showText_(true)
	, textEffectTime_(sf::Time::Zero)
{

	backgroundSprite_.setTexture(context.textures_->get(GEX::TextureID::TitleScreen));

	text_.setFont(GEX::FontManager::getInstance().get(GEX::FontID::Main));
	text_.setString("Press any key to start");
	centerOrigin(text_);

	text_.setPosition(context.window_->getView().getSize() / 2.f);

	context.music_->play(GEX::MusicID::MenuTheme);
}

//Draw the background and text
void TitleState::draw()
{
	auto& window = *getContext().window_;

	window.draw(backgroundSprite_);

	if (showText_) {
		window.draw(text_);
	}
}

//Blink the press key text
bool TitleState::update(sf::Time dt)
{
	textEffectTime_ += dt;

	if (textEffectTime_ >= sf::seconds(0.5f))
	{
		showText_ = !showText_;
		textEffectTime_ = sf::Time::Zero;
	}

	return true;
}

bool TitleState::handleEvent(const sf::Event & event)
{
	if (event.type != sf::Event::KeyPressed)
	{
		return false;
	}

	requestStackPop();
	requestStackPush(GEX::StateID::Game);

	return false;
}



