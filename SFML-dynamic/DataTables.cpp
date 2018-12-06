/**
* @file
* DataTables.cpp
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
#include "DataTables.h"
#include "JsonFrameParser.h"

namespace GEX
{
	std::map<GEX::ActorType, GEX::ActorData> GEX::initializeActorData()
	{
		std::map<ActorType, ActorData> data;

		JsonFrameParser frames = JsonFrameParser("Media/Textures/Atlas.json");

		// All actors must have a dead state otherwise the application will break when the actor is destroyed

		//Pacman
		data[ActorType::Pacman].texture = TextureID::Atlas;
		data[ActorType::Pacman].hitpoints = 100;
		data[ActorType::Pacman].speed = 50;
						
		data[ActorType::Pacman].animations[Actor::State::Walk].addFrameSet(frames.getFramesFor("pacman"));
		data[ActorType::Pacman].animations[Actor::State::Walk].setDuration(sf::Time(sf::seconds(1.f)));
		data[ActorType::Pacman].animations[Actor::State::Walk].setRepeating(true);
						
		data[ActorType::Pacman].animations[Actor::State::Dead].addFrameSet(frames.getFramesFor("die"));
		data[ActorType::Pacman].animations[Actor::State::Dead].setDuration(sf::Time(sf::seconds(1.f)));
		data[ActorType::Pacman].animations[Actor::State::Dead].setRepeating(false);

		//Cherry
		data[ActorType::Cherry].texture = TextureID::Atlas;
		data[ActorType::Cherry].hitpoints = 100;
		data[ActorType::Cherry].speed = 0;
												
		data[ActorType::Cherry].animations[Actor::State::Idle].addFrameSet(frames.getFramesFor("cherry"));
		data[ActorType::Cherry].animations[Actor::State::Idle].setDuration(sf::Time(sf::seconds(1.f)));
		data[ActorType::Cherry].animations[Actor::State::Idle].setRepeating(false);

		//Ghost
		data[ActorType::Ghost].texture = TextureID::Atlas;
		data[ActorType::Ghost].hitpoints = 100;
		data[ActorType::Ghost].speed = 50;
		
		data[ActorType::Ghost].animations[Actor::State::WalkUp].addFrameSet(frames.getFramesFor("ghost up"));
		data[ActorType::Ghost].animations[Actor::State::WalkUp].setDuration(sf::Time(sf::seconds(1.f)));
		data[ActorType::Ghost].animations[Actor::State::WalkUp].setRepeating(true);
						
		data[ActorType::Ghost].animations[Actor::State::WalkDown].addFrameSet(frames.getFramesFor("ghost down"));
		data[ActorType::Ghost].animations[Actor::State::WalkDown].setDuration(sf::Time(sf::seconds(1.f)));
		data[ActorType::Ghost].animations[Actor::State::WalkDown].setRepeating(true);

		data[ActorType::Ghost].animations[Actor::State::Dead].addFrameSet(frames.getFramesFor("ghost up"));
		data[ActorType::Ghost].animations[Actor::State::Dead].setDuration(sf::Time(sf::seconds(0.f)));
		data[ActorType::Ghost].animations[Actor::State::Dead].setRepeating(false);

		//Cherry
		data[ActorType::Power].texture = TextureID::Atlas;
		data[ActorType::Power].hitpoints = 100;
		data[ActorType::Power].speed = 0;
						
		data[ActorType::Power].animations[Actor::State::Idle].addFrameSet(frames.getFramesFor("cherry"));
		data[ActorType::Power].animations[Actor::State::Idle].setDuration(sf::Time(sf::seconds(1.f)));
		data[ActorType::Power].animations[Actor::State::Idle].setRepeating(false);

		data[ActorType::Power].animations[Actor::State::Dead].addFrameSet(frames.getFramesFor("cherry"));
		data[ActorType::Power].animations[Actor::State::Dead].setDuration(sf::Time(sf::seconds(0.f)));
		data[ActorType::Power].animations[Actor::State::Dead].setRepeating(false);

		return data;
	}

}
