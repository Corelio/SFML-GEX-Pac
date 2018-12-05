/**
* @file
* PostEffect.cpp
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
#include "PostEffect.h"
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/VertexArray.hpp>

namespace GEX
{

	bool PostEffect::isSupported()
	{
		return sf::Shader::isAvailable();
	}

	void PostEffect::applyShader(const sf::Shader & shader, sf::RenderTarget& output)
	{
		sf::Vector2f outputSize = static_cast<sf::Vector2f>(output.getSize());

		sf::VertexArray verticies(sf::TrianglesStrip, 4);
		verticies[0] = sf::Vertex(sf::Vector2f(0, 0), sf::Vector2f(0, 1));
		verticies[1] = sf::Vertex(sf::Vector2f(outputSize.x, 0), sf::Vector2f(1, 1));
		verticies[2] = sf::Vertex(sf::Vector2f(0, outputSize.y), sf::Vector2f(0, 0));
		verticies[3] = sf::Vertex(sf::Vector2f(outputSize), sf::Vector2f(1, 0));

		sf::RenderStates states;
		states.shader = &shader;
		states.blendMode = sf::BlendNone;

		output.draw(verticies, states);
	}
}