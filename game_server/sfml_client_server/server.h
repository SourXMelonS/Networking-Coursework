#pragma once
#include <SFML/Network.hpp>
#include <string>
#include <iostream>
#include <list>
#include <thread>
class server
{
public:
	void Init();
	void Update();
				
	//TCP
	void TCP();
	void TCPCommunicationHandler();
	void TCPMessageRecSend();
	void sendStartGame(sf::TcpSocket* socket_);
	void IdAndPositionSetter(sf::TcpSocket* socket_, std::string name_);
	void CoinRandomise();
			
	//UDP
	void UDP();
	void BindUDP();
	void receiveUDP();
	void sendUDP(sf::Packet receivePos_,int id_);
	void coinPickedEvent(sf::Packet packet_, int id_);
	//void checkDisconnections(sf::TcpSocket* sock);
	void sendTime();


protected:
	//Networking variables
	sf::UdpSocket UDP_socket;
	sf::TcpListener listener;
	sf::SocketSelector selector;
	std::vector<sf::TcpSocket*>clients;

	int coins_picked = 0;
	int id_setter = 0;
	bool connected;
	unsigned short UDP_port;

	//Struct for each player
	struct Player
	{
		sf::Vector2f startPos;
		int score = 0;
		sf::IpAddress ip;
		unsigned short portUDP;
		int clientID;
	};
	std::vector<Player> Players;			//Vector of struct

	//Different clocks
	sf::Clock startGameClock;
	sf::Clock gameClock;
	sf::Clock sendGameTime;

	bool gameStarted = false;

	float gameTime;
	bool gameStart = true;
	sf::Packet coinPositionPacket;
	bool generationDone=false;

	sf::Vector2f coinPos[20];			//how many coins

	enum types
	{
		namePacket = 1,
		chatToClients = 8,
		udpPosReceived = 3,
		disconnect = 6,
		coinPicked = 8,
		udpPosSent = 7,
		idAsked = 19,
		idSet = 1,
		coinGen = 2,
		coinPickedsendToClients = 9,
		sendStartGameType = 5,
		sendTimeType = 2,
	};
};

