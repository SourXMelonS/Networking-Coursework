#include "Menu.h"

Menu::Menu(RenderWindow* hwnd, Input* in, GameState* gs, sf::TcpSocket* sock)
{
	window = hwnd;
	input = in;
	gameState = gs;
	Tcp = sock;
	
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
	ipNameStr = "IP";
	joinTexture.loadFromFile("gfx/join.png");
	exitTexture.loadFromFile("gfx/exit.png");

	menuBackground.setSize(Vector2f(window->getSize().x, window->getSize().y));
	menuBackground.setFillColor(Color::Color(255, 0, 0, 255));
	//menuBackground.setTexture(&backgroundTexture);

	playButton = MenuButton(joinTexture, input, window, 1);
	exitButton = MenuButton(exitTexture, input, window, 2);

	gameName.setFont(font);
	gameName.setCharacterSize(90);
	gameName.setFillColor(Color::Color(255, 176, 102));
	gameName.setPosition(window->getSize().x / 2 - 550, 100);
	gameName.setString("Generic Coin Collect Game");

	ipEnter.setFont(font);
	ipEnter.setCharacterSize(50);
	ipEnter.setFillColor(Color::Color(255, 255, 255));
	ipEnter.setPosition(window->getSize().x / 4 - 100, 300);
	ipEnter.setString("Type Ip address to connect to");

	nameEnter.setFont(font);
	nameEnter.setCharacterSize(50);
	nameEnter.setFillColor(Color::Color(255, 255, 255));
	nameEnter.setPosition(window->getSize().x / 4 - 100, 400);
	nameEnter.setString("Type the name you would like to use");

	ipDisplay.setFont(font);
	ipDisplay.setCharacterSize(50);
	ipDisplay.setFillColor(Color::Color(255, 255, 255));
	ipDisplay.setPosition(window->getSize().x / 4 - 100, 350);
	

	nameDisplay.setFont(font);
	nameDisplay.setCharacterSize(50);
	nameDisplay.setFillColor(Color::Color(255, 255, 255));
	nameDisplay.setPosition(window->getSize().x / 4 - 100, 450);
	

}

void Menu::handleInput(float dt)
{
}

void Menu::handleInput(sf::Event* event_ )
{
	
		sf::Vector2u window_size = window->getSize();
		if (input->isKeyDown(sf::Keyboard::Escape))
		{
			window->close();
		}
		if (event_->type == sf::Event::TextEntered)		//Text is being entered
		{
			if (ipNameStr == "IP")				//IP text selected and entered
			{
				if (32 < event_->text.unicode && event_->text.unicode < 128)
				{
					ipEnterStr += (char)event_->text.unicode;
					std::cout << event_->text.unicode;
				}
			}
			else if (ipNameStr == "Name")	//name text selected and entered
			{
				if (32 < event_->text.unicode && event_->text.unicode < 128)
				{
					nameEnterStr += (char)event_->text.unicode;
				}
			}
		}
		if (event_->type == sf::Event::KeyPressed && ipNameStr == "IP")
		{
			enterIp(event_);
		}
		else if (event_->type == sf::Event::KeyPressed && ipNameStr == "Name")
		{
			enterName(event_);
		}
		ipDisplay.setString(ipEnterStr);
		nameDisplay.setString(nameEnterStr);

	
	
}

void Menu::update(float dt)
{
	if (gameState->getCurrentState() == State::MENU)
	{

		if (Collision::checkBoundingBox(&joinButton, (Vector2i(input->getMouseX(), input->getMouseY())))) {
			joinButton.collisionResponse(NULL);
			if (!playedButton3)
			{
				playedButton3 = true;
			}
		}
		else
		{
			joinButton.setTexture(&hostTexture);
			playedButton3 = false;
		}
		if (Collision::checkBoundingBox(&playButton, (Vector2i(input->getMouseX(), input->getMouseY()))))
		{
			playButton.collisionResponse(NULL);
			if (!playedButton1)
			{
				
				playedButton1 = true;
			}
		}
		else
		{
			playButton.setTexture(&joinTexture);
			playedButton1 = false;
		}

		if (Collision::checkBoundingBox(&exitButton, (Vector2i(input->getMouseX(), input->getMouseY())))) {
			exitButton.collisionResponse(NULL);
			if (!playedButton2)
			{
				playedButton2 = true;
			}
		}
		else
		{
			exitButton.setTexture(&exitTexture);
			playedButton2 = false;
		}
	}
}
void Menu::render()
{
	//Game title
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

void Menu::reset()
{
}

void Menu::enterName(sf::Event* event_)
{
	if (event_->key.code == sf::Keyboard::Return)		//Send message on enter
	{
		nameEnter.setFillColor(sf::Color::Green);
		sf::Packet name_sent;
		players += nameEnterStr;
		connect_attempt = true;
		ipNameStr == "";

		sf::Socket::Status Tcp_Stat = Tcp->connect(server_Ip, 53000);			//Once name is entered, connect to IP and port
		if (Tcp_Stat != sf::Socket::Done)
		{
			printf("Client couldn't connect'\n");
			printf("Server could be full, non existing or under maintenance\n");
			Init();			//If connection couldn't happen, restart menu
		}
		else
		{
			//If you managed to connect
		}
		{

			std::cout << "IS IT CONNECTED " << "\n";
			sendNameTCP();			//send name to server
			gameState->setCurrentState(State::LEVEL);			//Enter in game and game is started.
		}

	}
	else if (event_->key.code == sf::Keyboard::BackSpace)		//Removes last letter in the message in the chat
	{
		if (nameEnterStr.size() > 0)
			nameEnterStr.pop_back();
	}
	else if (event_->key.code == sf::Keyboard::Space)		//Space added
	{
		nameEnterStr += ' ';
	}
}

void Menu::enterIp(sf::Event* event_)
{
	if (event_->key.code == sf::Keyboard::Return)		//Send message on enter
	{
		server_Ip = sf::IpAddress(ipEnterStr);
		//ipAdress_server = sf::IpAddress::getLocalAddress();
		ipDisplay.setFillColor(sf::Color::Green);
		ipNameStr = "Name";
		renderJoinGame = true;		//Once IP is entered, enter name and render the button to join the game
	}
	else if (event_->key.code == sf::Keyboard::BackSpace)		//Removes last letter in the message in the chat
	{
		if (ipEnterStr.size() > 0)
			ipEnterStr.pop_back();
	}
	else if (event_->key.code == sf::Keyboard::Space)		//Space added
	{
		ipEnterStr += ' ';
	}
}

void Menu::sendNameTCP()
{
	printf("Connection successful!\n\n");
	sf::Packet name_sender;
	int type = sendName;
	name_sender << type;
	name_sender << nameEnterStr;
	std::cout << nameEnterStr << std::endl;
	if (Tcp->send(name_sender) != sf::Socket::Done)
	{
		std::cout << "Error setting up\n";
	}
}

sf::IpAddress Menu::getIp()
{
	return server_Ip;
}

std::string Menu::getPlayerName()
{
	return nameEnterStr;
}
