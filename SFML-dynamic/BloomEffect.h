/**
* @file
* BloomEffect.h
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
#include "PostEffect.h"
#include <array>
#include <map>
#include <memory>
#include <SFML/Graphics/RenderTexture.hpp>

namespace GEX
{
	class BloomEffect : public PostEffect
	{
	public:
		enum class Shaders
		{
			BrightnessPass,
			DownSamplePass,
			GaussianBlurPass,
			AddPass,
		};

	private:
		typedef std::array<sf::RenderTexture, 2> RenderTextureArray;

	public:
		BloomEffect();

		void											apply(const sf::RenderTexture& input, sf::RenderTarget& output) override;

	private:
		void											prepareTextures(sf::Vector2u size);
		void											filterBright(const sf::RenderTexture& input, sf::RenderTexture& output);
		void											blurMultipass(RenderTextureArray& renderTextures);
		void											blur(const sf::RenderTexture& input, sf::RenderTexture& output, sf::Vector2f offsetFactor);
		void											downSample(const sf::RenderTexture& input, sf::RenderTexture& output);
		void											add(const sf::RenderTexture& source, const sf::RenderTexture& bloom, sf::RenderTarget& target);

	private:
		std::map<Shaders, std::unique_ptr<sf::Shader>>	shaders_;

		sf::RenderTexture								brightnessTexture_;
		RenderTextureArray								firstPassTexture_;
		RenderTextureArray								secondPassTexture_;
	};
}
