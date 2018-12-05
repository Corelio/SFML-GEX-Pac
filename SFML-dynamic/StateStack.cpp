/**
* @file
* StateStack.cpp
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
#include "StateStack.h"
#include <cassert>

namespace GEX
{
	StateStack::StateStack(State::Context context)
		: context_(context)
		, pendingList_()
		, stack_()
		, factories_()
	{

	}

	StateStack::~StateStack()
	{
		
	}

	void StateStack::update(sf::Time dt)
	{
		for (auto itr = stack_.rbegin(); itr != stack_.rend(); ++itr)
		{
			if (!(*itr)->update(dt))
			{
				break;
			}
		}
		applyPendingChanges();
	}

	void StateStack::draw()
	{
		for (auto& itr : stack_)
		{
			itr->draw();
		}
	}

	void StateStack::handleEvent(const sf::Event & event)
	{
		for (auto itr = stack_.rbegin(); itr != stack_.rend(); ++itr)
		{
			if (!(*itr)->handleEvent(event))
			{
				break;
			}
		}

		applyPendingChanges();
	}

	void StateStack::pushState(GEX::StateID stateID)
	{
		pendingList_.push_back(PendingChange(Action::Push, stateID));
	}

	void StateStack::popState()
	{
		pendingList_.push_back(PendingChange(Action::Pop));
	}

	void StateStack::clearStates()
	{
		pendingList_.push_back(PendingChange(Action::Clear));
	}

	bool StateStack::isEmpty() const
	{
		return stack_.empty();
	}

	State::Ptr StateStack::createState(GEX::StateID stateID)
	{
		auto found = factories_.find(stateID);
		assert(found != factories_.end());

		return found->second();
	}

	void StateStack::applyPendingChanges()
	{
		for (auto itr : pendingList_)
		{
			switch (itr.action)
			{
			case Action::Push:
				stack_.push_back(createState(itr.stateID));
				break;
			case Action::Pop:
				stack_.pop_back();
				break;
			case Action::Clear:
				stack_.clear();
				break;
			default:
				break;
			}
		}

		pendingList_.clear();
	}

	StateStack::PendingChange::PendingChange(Action action, StateID stateID)
		: action(action)
		, stateID(stateID)
	{
	}
}