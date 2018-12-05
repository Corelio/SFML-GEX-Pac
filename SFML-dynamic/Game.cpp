#include "Game.h"

//homework
/*
	add keys j & k keys to rotate right and left
	other key to scale 
*/

namespace {
	const sf::IntRect NOROLL(0, 0, 48, 64);
	const sf::IntRect ROLLLEFT(48, 0, 48, 64);
	const sf::IntRect ROLLRIGHT(96, 0, 48, 64);
}

namespace GEX
{
	Game::Game()
		: window_(sf::VideoMode(800, 600), "Killer Planes!")
		, world_(window_)
		, statisticsText_()
		, statisticsUpdateTime_(sf::Time::Zero)
		, statisticsNumFrames_(0)
	{
		/*loadTextures();

		player_.setPosition(300, 200);
		player_.setTexture(textures_.get(GEX::TextureID::Airplanes));
		player_.setTextureRect(NOROLL);
		auto origin = player_.getOrigin();
		auto size = player_.getLocalBounds();
		player_.setOrigin(origin.x+size.width/2.f, origin.y+size.height/2.f);
		*/

		if (!font_.loadFromFile("Media/Sansation.ttf"))
		{
			// Handle loading font error
		}

		statisticsText_.setFont(font_);
		statisticsText_.setPosition(5.0f, 5.0f);
		statisticsText_.setCharacterSize(12.0f);
		statisticsText_.setString("Frames / Second = \nTime / Update =");
	}


	Game::~Game()
	{
	}

	void Game::run()
	{

		sf::Clock clock;

		sf::Time timeSinceLastUpdate = sf::Time::Zero;

		while (window_.isOpen())
		{
			processInputs();
			/*
			timeSinceLastUpdate += clock.restart();
			if (timeSinceLastUpdate >= SPF)
			{
				update(SPF);
				updateStatistics(timeSinceLastUpdate);
				timeSinceLastUpdate -= SPF;
			}else{
				update(timeSinceLastUpdate);
				updateStatistics(timeSinceLastUpdate);
				timeSinceLastUpdate -= timeSinceLastUpdate;
			}
			*/
			timeSinceLastUpdate = clock.restart();
			while (timeSinceLastUpdate > SPF)
			{
				update(SPF);
				updateStatistics(timeSinceLastUpdate);
				timeSinceLastUpdate -= SPF;
			}
			update(timeSinceLastUpdate);
			updateStatistics(timeSinceLastUpdate);

			render();
		}
	}

	void Game::processInputs()
	{

		CommandQueue& commands = world_.getCommandQueue();

		sf::Event event;
		while (window_.pollEvent(event))
		{

			player_.handleEvent(event, commands);

			if (event.type == sf::Event::Closed)
			{
				window_.close();
			}
		}

		player_.handleRealTimeInput(commands);

	}

	void Game::update(sf::Time deltaTime)
	{
		world_.update(deltaTime);
		//float timeDelta = 0.05;
		//sf::Vector2f moveDelta(velocity_.x * deltaTime.asSeconds(), velocity_.y * deltaTime.asSeconds());

		/*
		//player_.move(velocity_*deltaTime.asSeconds());

		//auto p = player_.getPosition();
		//p.x = p.x > 600 ? 0 : p.x;
		//p.x = p.x < 0 ? 600 : p.x;
		//p.y = p.y > 400 ? 0 : p.y;
		//p.y = p.y < 0 ? 400 : p.y;

		//player_.setPosition(p);

		//player_.rotate(rotation_ * deltaTime.asSeconds());

		//player_.setScale(scale_);

		//player_.setTextureRect(NOROLL);
		*/
		/*
		if (velocity_.x > 0) {
			player_.setTextureRect(ROLLRIGHT);
		}
		if (velocity_.x < 0) {
			player_.setTextureRect(ROLLLEFT);
		}
		*/
		//Debug
		//std::cout << "v(x): " << velocity_.x << ", v(y): " << velocity_.y  << ", r: " << rotation_  << ", s: " << scale_ << std::endl;
	}

	void Game::render()
	{

		window_.clear();

		world_.draw();
		window_.setView(window_.getDefaultView());
		window_.draw(statisticsText_);
		window_.display();

	}


	void Game::updateStatistics(sf::Time deltaTime)
	{
		statisticsUpdateTime_ += deltaTime;
		statisticsNumFrames_++;

		if (statisticsUpdateTime_ > sf::seconds(1))
		{
			statisticsText_.setString(
				"Frames / Second = " + std::to_string(statisticsNumFrames_) + "\n" +
				"Time / Update   = " + std::to_string(statisticsUpdateTime_.asMicroseconds() / statisticsNumFrames_) + "ms"
			);
			statisticsNumFrames_ = 0;
			statisticsUpdateTime_ -= sf::seconds(1);
		}


	}
	/*
	void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
	{

		const float PLAYER_SPEED = 10;

		if (!isPressed) {
			return;
		}
		if (key == sf::Keyboard::Up)
		{
			velocity_ += sf::Vector2f(0, -PLAYER_SPEED);
		}
		if (key == sf::Keyboard::Down)
		{
			velocity_ += sf::Vector2f(0, PLAYER_SPEED);
		}
		if (key == sf::Keyboard::Right)
		{
			velocity_ += sf::Vector2f(PLAYER_SPEED, 0);
		}
		if (key == sf::Keyboard::Left)
		{
			velocity_ += sf::Vector2f(-PLAYER_SPEED, 0);
		}
		if (key == sf::Keyboard::J)
		{
			//rotation_ += -PLAYER_SPEED;  //Disabiling rotation
		}
		if (key == sf::Keyboard::K)
		{
			//rotation_ += PLAYER_SPEED;  //Disabiling rotation
		}
		if (key == sf::Keyboard::Q)
		{
			scale_-=sf::Vector2f(0.1, 0.1);
		}
		if (key == sf::Keyboard::W)
		{
			scale_+=sf::Vector2f(0.1, 0.1);
		}
		if (key == sf::Keyboard::Escape)
		{
			rotation_=0;
			velocity_ = sf::Vector2f(0, 0);
			scale_ = sf::Vector2f(1.f,1.f);
		}
	}
	*/
	/*
	void Game::drawBoundingBox()
	{
		auto rect = player_.getGlobalBounds();

		sf::RectangleShape boundingBox;
		boundingBox.setPosition(sf::Vector2f(rect.left, rect.top));
		boundingBox.setSize(sf::Vector2f(rect.width, rect.height));
		boundingBox.setFillColor(sf::Color::Transparent);
		boundingBox.setOutlineColor(sf::Color::White);
		boundingBox.setOutlineThickness(1.f);

		window_.draw(boundingBox);
	}

	void Game::loadTextures()
	{
		textures_.load(GEX::TextureID::Airplane, "Media/Textures/Eagle.png");
		textures_.load(GEX::TextureID::Airplanes, "Media/Textures/Eagles.png");
		textures_.load(GEX::TextureID::Landscape, "Media/Textures/Desert.png");
		textures_.load(GEX::TextureID::Space, "Media/Textures/Space.png");

	}
	*/
}