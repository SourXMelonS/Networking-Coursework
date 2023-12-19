#pragma once
#include <SFML/Network.hpp>
#include <string>
#include <iostream>
#include <list>
#include<thread>
class server
{
public:
	void Init();
	void Update();
				
	//UDP
	void TCP();
	void TCPCommunicationHandler();
	void TCPMessageRecSend();
	void sendStartGame(sf::TcpSocket* sock);
	void IdAndPositionSetter(sf::TcpSocket* sock, std::string name_);
			
	//TCP
	void UDP();
	void BindUDP();
	void receiveUDP();
	void sendUDP(sf::Packet receivePosVar,int ID);
	//void coinPickedEvent(sf::Packet pack, int id);
	//void checkDisconnections(sf::TcpSocket* sock);
	void sendTime();


protected:
	//Networking variables
	sf::UdpSocket UDP_socket;
	sf::TcpListener listener;
	sf::SocketSelector selector;
	std::vector<sf::TcpSocket*>clients;


	int id_setter=26;
	bool connected;
	int tcp_port;
	unsigned short udp_port;
	bool server_started;
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
	sf::Clock discCheckClock;
	sf::Clock howOftenSendGameTime;

	bool gameStarted = false;

	float gameTime;
	bool gameStart = true;
	sf::Packet boxPosPacket;
	bool genDone=false;

	sf::Vector2f boxPos[10];			//how many coins

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
		boxGen = 2,
		coinPickedsendToClients = 9,
		sendStartGameType = 5,
		sendTimeType = 2,
	};
};

