#include "Menu.h"

Menu::Menu(RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud)
{
	window = hwnd;
	input = in;
	audio = aud;
	gameState = gs;

	
}
Menu::~Menu()
{


}

void Menu::Init()
{
	font.loadFromFile("font/arial.ttf");

	if (!font.loadFromFile("font/arial.ttf"))
	{
		std::cout << "Font not found" << std::endl;
	}

	ipEnterStr = "";
	nameEnterStr = "";

	renderJoinGame = false;
	ipName = "IP";
	backgroundTexture.loadFromFile("gfx/....png");
	hostTexture.loadFromFile("gfx/....png");
	joinTexture.loadFromFile("gfx/....png");

	menuBackground.setSize(Vector2f(window->getSize().x, window->getSize().y));
	menuBackground.setTexture(&backgroundTexture);

	joinButton = MenuButton(hostTexture, input, window, 1);
	playButton = MenuButton(joinTexture, input, window, 2);

	gameName.setFont(font);
	gameName.setCharacterSize(100);
	gameName.setFillColor(Color::Color(255, 176, 102));
	gameName.setPosition(window->getSize().x / 2 - 350, 100);
	gameName.setString("Generic Racing Game");

	ipEnter.setFont(font);
	ipEnter.setCharacterSize(50);
	ipEnter.setFillColor(Color::Color(255, 255, 255));
	ipEnter.setPosition(window->getSize().x / 2 - 100, 300);
	ipEnter.setString("Type Ip address to connect to");

	nameEnter.setFont(font);
	nameEnter.setCharacterSize(50);
	nameEnter.setFillColor(Color::Color(255, 255, 255));
	nameEnter.setPosition(window->getSize().x / 2 - 100, 400);
	nameEnter.setString("Type the name you would like to use");

	ipDisplay.setFont(font);
	ipDisplay.setCharacterSize(50);
	ipDisplay.setFillColor(Color::Color(255, 255, 255));
	ipDisplay.setPosition(window->getSize().x / 2 - 100, 350);
	

	nameDisplay.setFont(font);
	nameDisplay.setCharacterSize(50);
	nameDisplay.setFillColor(Color::Color(255, 255, 255));
	nameDisplay.setPosition(window->getSize().x / 2 - 100, 450);
	

}

void Menu::handleInput(float dt)
{
}

void Menu::handleInput(float dt, sf::Event* event_)
{
	if (gameState->getCurrentState() == State::MENU)
	{
		if (Collision::checkBoundingBox(&joinButton, (Vector2i(input->getMouseX(), input->getMouseY()))) && (input->isLeftMouseDown()))
		{
			audio->stopAllMusic();
			gameState->setCurrentState(State::HOST);
		}
		if (Collision::checkBoundingBox(&playButton, (Vector2i(input->getMouseX(), input->getMouseY()))) && (input->isLeftMouseDown()))
		{
			audio->stopAllMusic();
			gameState->setCurrentState(State::JOIN);
		}
		//If the player clicks on exit the game closes
		if (Collision::checkBoundingBox(&exitButton, (Vector2i(input->getMouseX(), input->getMouseY()))) && (input->isLeftMouseDown()))
		{
			window->close();
		}
	}
	if (gameState->getCurrentState() == State::JOIN)
	{
		sf::Vector2u window_size = window->getSize();
		if (input->isKeyDown(sf::Keyboard::Escape))
		{
			window->close();
		}

		if (event_->type = sf::Event::TextEntered)
		{
			if (ipName == "IP")
			{
				if (32 < event_->text.unicode && event_->text.unicode < 128)
				{
					ipEnterStr += (char)event_->text.unicode;
				}
			}
			else if (ipName == "Name")
			{
				if (32 < event_->text.unicode && event_->text.unicode < 128)
				{
					nameEnterStr += (char)event_->text.unicode;
				}
			}
		}
		if (event_->type == sf::Event::KeyPressed && ipName == "IP")
		{
			enterIp(event_);
		}
		else if (event_->type == sf::Event::KeyPressed && ipName == "Name")
		{
			enterName(event_);
		}
		ipDisplay.setString(ipEnterStr);
		nameDisplay.setString(nameEnterStr);

	}
	
}

void Menu::update(float dt)
{
	if (gameState->getCurrentState() == State::MENU)
	{

		if (Collision::checkBoundingBox(&joinButton, (Vector2i(input->getMouseX(), input->getMouseY())))) {
			joinButton.collisionResponse(NULL);
			if (!playedButton1)
			{
				playedButton1 = true;
			}
		}
		else
		{
			joinButton.setTexture(&hostTexture);
			playedButton1 = false;
		}
		if (Collision::checkBoundingBox(&playButton, (Vector2i(input->getMouseX(), input->getMouseY()))))
		{
			playButton.collisionResponse(NULL);
			if (!playedButton2)
			{
				playedButton2 = true;
			}
		}
		else
		{
			playButton.setTexture(&joinTexture);
			playedButton2 = false;
		}

		if (Collision::checkBoundingBox(&exitButton, (Vector2i(input->getMouseX(), input->getMouseY())))) {
			exitButton.collisionResponse(NULL);
			if (!playedButton3)
			{
				playedButton3 = true;
			}
		}
		else
		{
			exitButton.setTexture(&exitTexture);
			playedButton3 = false;
		}
	}
}
void Menu::render()
{
	//Game title
	
	if (gameState->getCurrentState() == State::MENU)
	{
		beginDraw();
		window->draw(menuBackground);
		window->draw(gameName);
		window->draw(joinButton);
		window->draw(playButton);
		window->draw(exitButton);
		endDraw();
	}
	if (gameState->getCurrentState() == State::JOIN)
	{
		beginDraw();
		window->draw(menuBackground);
		window->draw(gameName);
		window->draw(ipEnter);
		window->draw(ipDisplay);
		window->draw(nameEnter);
		window->draw(nameDisplay);
		if (renderJoinGame == true)
		{
			window->draw(joinButton);
		}
		endDraw();
	}
}

void Menu::reset()
{
}

void Menu::enterName(sf::Event* event_)
{
}

void Menu::enterIp(sf::Event* event_)
{
}

void Menu::sendNameTCP()
{
}

sf::IpAddress Menu::getIp()
{
	return sf::IpAddress();
}

std::string Menu::getPlayerName()
{
	return std::string();
}
