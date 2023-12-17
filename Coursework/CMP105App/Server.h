#pragma once
#include <SFML/Network.hpp>
#include <string>
#include <iostream>
#include <list>
#include <thread>

class Server
{
public:
	void Init();
	void Update();
protected:
	//------------------TCP FUNCTIONS
	void TCP();
	void TCPCommunicationHandler();
	void TCPMessageRecSend();
	void sendStartGame(sf::TcpSocket* sock);
	void IdAndPositionSetter(sf::TcpSocket* sock, std::string name_);
	//------------------UDP FUNCTIONS
	void UDP();
	void BindUDP();
	void receiveUDP();
	void sendUDP(sf::Packet receivePosVar, int ID);
	//	void checkDisconnections(sf::TcpSocket* sock);
	void sendTime();

protected:
	//NETWORKING VARIABLES
	sf::UdpSocket UDP_socket;
	sf::TcpListener listener;
	sf::SocketSelector selector;
	std::vector<sf::TcpSocket*>clients;

	int id_setter = 26;
	bool connected;
	int tcp_port;
	unsigned short udp_port;
	bool server_started;
	//Struct for each player
	struct Player
	{
		sf::Vector2f position;
		sf::IpAddress ip;
		unsigned short portUDP;
		int clientID;
	};
	std::vector<Player> Players;

	//Different clocks
	sf::Clock startGameClock;
	sf::Clock gameClock;
	sf::Clock discCheckClock;
	sf::Clock howOftenSendGameTime;

	bool gameStarted = false;

	float gameTime;
	bool gameStart = true;
	bool genDone = false;

	enum types
	{
		namePacket = 1,
		chatToClients = 8,
		udpPosReceived = 3,
		disconnect = 6,
		udpPosSent = 7,
		idAsked = 19,
		idSet = 1,
		sendStartGameType = 5,
		sendTimeType = 2,
	};
};


