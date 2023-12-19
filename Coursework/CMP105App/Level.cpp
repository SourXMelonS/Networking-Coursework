#include "Level.h"



Level::Level(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud, sf::TcpSocket* sock)
{
	window = hwnd;
	input = in;
	gameState = gs;
	audio = aud;
	connected_succesfully = false;
	Tcp = sock;
	// initialise game objects

}

Level::~Level()
{

}

void Level::Init(Menu* menu_)
{
	port = 53000;
	menu = menu_;
	name = menu->getPlayerName();
	IP_ADDRESS = menu->getIp();
	client_ = std::make_unique<Client>(IP_ADDRESS, port, player, name, Tcp, window, input);
	connected_succesfully = client_->getConnectedStatus();


	if (connected_succesfully == true)
	{
		player.Init();
	}
	else if (connected_succesfully == false)
	{
		menu_->Init();
		gameState->setCurrentState(State::MENU);
	}

}
// handle user input
void Level::handleInput(float dt)
{
	if (connected_succesfully == true)
	{
		player.handleInput(input,dt);
	}
}

void Level::update(float dt)
{
	
}

// Update game objects
void Level::update(float dt, sf::Event* event_)
{
	if (client_->someoneJoined)
	{
		client_->someoneJoined = false;
		opponents.push_back(client_->getLastAddedP());
	}
	if (connected_succesfully == true)
	{
		client_->Update(input, event_, &player, dt);
		player.update();
		for (int i = 0; i < opponents.size(); i++)
		{
			opponents.at(i).update();
			opponents.at(i).setPosition(client_->getClientPos(i));
		}
	}
}

void Level::clientInput(sf::Event* event_)
{
}

// Render level
void Level::render()
{
	beginDraw();
	client_->Render();
	player.render(window);
	for (int i = 0; i < opponents.size(); i++)
		opponents.at(i).render(window);
	endDraw();
}



//Player Level::getPlayer(int player_)
//{
	//return player[player_];
//}

