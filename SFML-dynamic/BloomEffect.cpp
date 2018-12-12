/**
* @file
* BloomEffect.cpp
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
#include "BloomEffect.h"

#include <string>
#include <cassert>
#include <SFML/Graphics/Shader.hpp>

namespace GEX
{
	BloomEffect::BloomEffect()
		: shaders_()
		, brightnessTexture_()
		, firstPassTexture_()
		, secondPassTexture_()
	{


		std::unique_ptr<sf::Shader> s(new sf::Shader()); 
		std::string f1name = "Media/Shaders/Fullpass.vert";
		std::string f2name = "Media/Shaders/Brightness.frag";
		if (!(s->loadFromFile("Media/Shaders/Fullpass.vert", "Media/Shaders/Brightness.frag")))
			throw std::runtime_error("Shader::load - Failed to load ");
		auto inserted = shaders_.insert(std::make_pair(Shaders::BrightnessPass, std::move(s)));
		assert(inserted.second);

		s.reset(new sf::Shader());
		f1name = "Media/Shaders/Fullpass.vert";
		f2name = "Media/Shaders/DownSample.frag";
		if (!s->loadFromFile(f1name, f2name))
			throw std::runtime_error("Shader::load - Failed to load " + f1name);
		inserted = shaders_.insert(std::make_pair(Shaders::DownSamplePass, std::move(s)));
		assert(inserted.second);

		s.reset(new sf::Shader());
		f1name = "Media/Shaders/Fullpass.vert";
		f2name = "Media/Shaders/GuassianBlur.frag";
		if (!s->loadFromFile(f1name, f2name))
			throw std::runtime_error("Shader::load - Failed to load " + f1name);
		inserted = shaders_.insert(std::make_pair(Shaders::GaussianBlurPass, std::move(s)));
		assert(inserted.second);

		s.reset(new sf::Shader());
		f1name = "Media/Shaders/Fullpass.vert";
		f2name = "Media/Shaders/Add.frag";
		if (!s->loadFromFile(f1name, f2name))
			throw std::runtime_error("Shader::load - Failed to load " + f1name);
		inserted = shaders_.insert(std::make_pair(Shaders::AddPass, std::move(s)));
		assert(inserted.second);

	}

	void BloomEffect::apply(const sf::RenderTexture& input, sf::RenderTarget& output)
	{
		prepareTextures(input.getSize());

		filterBright(input, brightnessTexture_);

		downSample(brightnessTexture_, firstPassTexture_[0]);
		blurMultipass(firstPassTexture_);

		downSample(firstPassTexture_[0], secondPassTexture_[0]);
		blurMultipass(secondPassTexture_);

		add(firstPassTexture_[0], secondPassTexture_[0], firstPassTexture_[1]);
		firstPassTexture_[1].display();

		add(input, firstPassTexture_[1], output);
	}

	void BloomEffect::prepareTextures(sf::Vector2u size)
	{
		if (brightnessTexture_.getSize() != size)
		{
			brightnessTexture_.create(size.x, size.y);
			brightnessTexture_.setSmooth(true);

			firstPassTexture_[0].create(size.x / 2, size.y / 2);
			firstPassTexture_[0].setSmooth(true);
			firstPassTexture_[1].create(size.x / 2, size.y / 2);
			firstPassTexture_[1].setSmooth(true);

			secondPassTexture_[0].create(size.x / 4, size.y / 4);
			secondPassTexture_[0].setSmooth(true);
			secondPassTexture_[1].create(size.x / 4, size.y / 4);
			secondPassTexture_[1].setSmooth(true);
		}
	}

	void BloomEffect::filterBright(const sf::RenderTexture& input, sf::RenderTexture& output)
	{
		sf::Shader& brightness = *shaders_.at(Shaders::BrightnessPass);

		brightness.setUniform("source", input.getTexture());
		applyShader(brightness, output);
		output.display();
	}

	void BloomEffect::blurMultipass(RenderTextureArray& renderTextures)
	{
		sf::Vector2u textureSize = renderTextures[0].getSize();

		for (std::size_t count = 0; count < 2; ++count)
		{
			blur(renderTextures[0], renderTextures[1], sf::Vector2f(0.f, 1.f / textureSize.y));
			blur(renderTextures[1], renderTextures[0], sf::Vector2f(1.f / textureSize.x, 0.f));
		}
	}

	void BloomEffect::blur(const sf::RenderTexture& input, sf::RenderTexture& output, sf::Vector2f offsetFactor)
	{
		sf::Shader& gaussianBlur = *shaders_.at(Shaders::GaussianBlurPass);

		gaussianBlur.setUniform("source", input.getTexture());
		gaussianBlur.setUniform("offsetFactor", offsetFactor);
		applyShader(gaussianBlur, output);
		output.display();
	}

	void BloomEffect::downSample(const sf::RenderTexture& input, sf::RenderTexture& output)
	{
		sf::Shader& downSampler = *shaders_.at(Shaders::DownSamplePass);

		downSampler.setUniform("source", input.getTexture());
		downSampler.setUniform("sourceSize", sf::Vector2f(input.getSize()));
		applyShader(downSampler, output);
		output.display();
	}

	void BloomEffect::add(const sf::RenderTexture& source, const sf::RenderTexture& bloom, sf::RenderTarget& output)
	{
		sf::Shader& adder = *shaders_.at(Shaders::AddPass);

		adder.setUniform("source", source.getTexture());
		adder.setUniform("bloom", bloom.getTexture());
		applyShader(adder, output);
	}
}
