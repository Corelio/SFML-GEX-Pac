/**
* @file
* JsonFrameParser.cpp
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
#include "JsonFrameParser.h"
#include <fstream>
#include <iostream>

using json = nlohmann::json;


JsonFrameParser::JsonFrameParser(std::string path)
{
	std::ifstream ifs(path);
	json_ = json::parse(ifs);
}

std::vector<sf::IntRect>  JsonFrameParser::getFramesFor(std::string animationName) const
{
	std::vector<sf::IntRect> data; // frame textRecs for animaionName in atlas

	json k = json_["frames"];

	for (auto i : k)
	{

		std::string tmpStr = i["filename"]; // animation name is the first part of "filename" string
		if (tmpStr.compare(0, animationName.size(), animationName) == 0)
		{
			data.push_back(sf::IntRect(i["frame"]["x"],
				i["frame"]["y"],
				i["frame"]["w"],
				i["frame"]["h"]));
		}
	}

	return data;
}
