#pragma once
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include "Framework/Input.h"
#include <string>
#include <sstream>      // std::stringstream
#include <iostream>
#include <stdlib.h>
#include "Player.h"
//#include "GraphicsTools.h"
#include "Framework/Collision.h"
#include <queue>

class Client
{
	Client(sf::IpAddress& ip, unsigned short& port, Player& p, std::string& name, sf::TcpSocket* socket, sf::RenderWindow* window);
	~Client();
	void HandleInput(sf::Event* Ev, Input* input, Player* p);		//Handle Input for inputs 
	void Update(Input* input, sf::Event* Ev, Player* p, float dt);
	void Render();
	void disconnect(Player* p, Input* input);		//Disconnect once esc is pressed
	void Setup(Player* p);

	void sendMessageTCP(Player* p);		//------------------------------------------TCP FUNCTIONS
	void TCPReceive();
	void ID_And_Positions_Getter();
	void textSetup(sf::RenderWindow* window);

	void UDP_sendPosition(Player* p, Input* input, float dt);		//------------------------------------------UDP FUNCTIONS
	void UDPReceive(Player* p);
	void interpolateEnemyPos(Player* Player, float dt);
	void CheckCollision(Player* p);

	sf::Vector2f getClientPos(int idx) {			//Get client position at a certain index
		return enemies.at(idx).getPosition();
	}
};

