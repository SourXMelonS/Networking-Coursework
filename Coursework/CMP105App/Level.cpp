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

void Level::Init()
{
	port = 53000;

}
// handle user input
void Level::handleInput(float dt)
{
	
}

void Level::update(float dt)
{
}

// Update game objects
void Level::update(float dt, sf::Event* event_)
{
	
}

void Level::clientInput(sf::Event* event_)
{
}

// Render level
void Level::render()
{
	beginDraw();

	endDraw();
}



//Player Level::getPlayer(int player_)
//{
	//return player[player_];
//}

