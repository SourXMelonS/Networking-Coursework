#pragma once
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include "Framework/Input.h"
#include <string>
#include <sstream>      // std::stringstream
#include <iostream>
#include <stdlib.h>
#include "Player.h"
#include "Framework/Collision.h"
#include <queue>
#include "Coins.h"

class Client
{
public:
	Client();
	Client(sf::IpAddress& ip, unsigned short& port, Player& p, std::string& name, sf::TcpSocket* socket, sf::RenderWindow* window, Input* input);
	~Client();
	//void Init();
	void HandleInput(sf::Event* Ev, Input* input, Player* p);		//Handle Input for inputs 
	void Update(Input* input, sf::Event* Ev,Player* p, float dt);
	void Render();
	void disconnect(Player* p, Input* input);		//Disconnect once esc is pressed
	void Setup(Player* p);
	void RenderBackgound();

	void sendMessageTCP(Player* p);		//------------------------------------------TCP FUNCTIONS
	void TCPReceive();
	void ID_And_Positions_Getter();
	void textSetup(sf::RenderWindow* window);

	void UDP_sendPosition(Player* p, Input* input, float dt);		//------------------------------------------UDP FUNCTIONS
	void UDPReceive(Player* p);
	void interpolateOpponentPos(Player* Player, float dt);
	void CheckCollision(Player* p);

	void askSetup();
	void generateCoin();
	void coinPosGetterTCP();
	void coinPosGetterUDP();
	bool getConnectedStatus();

	Coins coins[20];
	Vector2f getClientPos(int id)
	{
		return opponents.at(id).getPosition();
	}
	Player getLastAddedP() { return opponents.back(); }
	bool someoneJoined = false;
	sf::Vector2f next_pos_pass_to_level;
	
protected:
	std::string name;
	std::string userText;
	std::string timeDisplay;
	sf::Font font;

	sf::TcpSocket* socket;
	sf::UdpSocket udp_socket;
	std::vector<sf::Text>chat;
	sf::IpAddress Ip_serverAddress;

	Input* input;

	sf::Text drawText;
	sf::Text timeText;
	sf::Text scoreText;

	sf::Clock clock;
	sf::RenderWindow* window_;

	unsigned short udp_port;

	int id_getter;
	int coins_picked_up =0;
	bool connected_;
	bool open_chat;		
	bool chat_empty_on_open;	
	bool is_chat_open = false;
	float lerpAlpha;
	float deltaTime;

	Vector2f coinPos[20];
	Player player;
	std::vector<Player>opponents;

	enum dataInfo				
	{
		askForSetup = 19,
		sendPlayerPos = 3,
		gameTimeReceive = 2,
		recieveOpponentPos = 7,
		coinHasBeenPicked = 9,
		sendDisconnection = 6,
		IdReceived = 1,
		startGameType = 5,
		chatReceived = 8,
		coinPositions = 2
	};

	float score;
	float gameTime;
};

