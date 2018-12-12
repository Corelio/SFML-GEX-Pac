/**
* @file
* JsonFrameParser.h
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

#include "json.hpp"
#include <vector>
#include <SFML/Graphics/Rect.hpp>

using nlohmann::json;

struct IntRect {

	IntRect(int l, int t, int w, int h) : left(l), top(t), width(w), height(h) {}
	int left;
	int top;
	int width;
	int height;
};

class JsonFrameParser
{
public:
									/**
									* Construct a jason frame parser object
									*
									* @param[in] path path to texture atlas
									*/
									JsonFrameParser(std::string path);

									/**
									* Return set of texture Rectangles for the named
									* animation. 
									* 
									* @param animationName the name of the animation, must match 
									*		 the begining of the name string for the animation in the 
									*		 json file.
									*
									* @return vector of Frames that make up the animation 
									*/
	std::vector<sf::IntRect>        getFramesFor(std::string animationName) const;

private:
	json              json_;
};



