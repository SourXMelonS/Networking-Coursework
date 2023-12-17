#include "Server.h"

void Server::Init()
{
	std::cout << "The local server address is: \n" << sf::IpAddress::getLocalAddress() << "\n";			//Local address for local connection and testing
	std::cout << "The public server address is: \n" << sf::IpAddress::getPublicAddress() << "\n";		//Public address to test outside local connection
	udp_port = 54000;			//UDP port hardcoded
	listener.listen(53000);			//TCP port hardcoded
	listener.setBlocking(false);			//Listener doesn't block so that it doesn't stop events from happening 
	selector.add(listener);			//Selector adds listener, necessary for multiple connections
	UDP_socket.setBlocking(false);			//UDP socket set to not blocking, 
	srand(time(NULL));
	BindUDP();			//Bind UDP to a port.
}

void Server::Update()
{

	bool done = false;
	while (!done)		//infinite loop
	{
		TCP();			//TCP handled in main update
		UDP();			//UDP handled in main update
		sendTime();		//after UDP and TCP, send the game time to everyone.
	}

}
void Server::TCP()
{
	TCPCommunicationHandler();
}

void Server::TCPCommunicationHandler()
{

	sf::TcpSocket* socket = new sf::TcpSocket;
	if (listener.accept(*socket) != sf::Socket::Done)			
	{
		std::cout << "error accepting new conn\n";
	}
	else
	{
		if (clients.size() < 2)	//maximum server size 2
		{
			selector.add(*socket);			
			sf::Packet name_packet;
			std::string name;
			if (socket->receive(name_packet) == sf::Socket::Done) //store name to display who connected to the server
			{
				int type = 0;
				name_packet >> type;
				if (type == namePacket)	//Packet type is 1
				{
					name_packet >> name;
					std::cout << name << " has connected to the chat room. " << std::endl;			//display who connected
				}
				IdAndPositionSetter(socket, name);			//when someone connects, set its positions and id
			}

			socket->setBlocking(false);			//Each socket is not blocking
			clients.push_back(socket);			//Socket is added to the clients vector
			connected = true;
		}
	}
	TCPMessageRecSend();
}

void Server::TCPMessageRecSend()		//Handles all the TCP events
{
	
	for (int i = 0; i < clients.size(); i++)		//For all the clients
	{
		sf::Packet packet, sendPacket;
		if (clients[i]->receive(packet) == sf::Socket::Done)		//If a message is received
		{
			std::cout << "someone sent a message\n";			//Display to debug
			std::string text;
			packet >> text;			//Message is extracted
			int type = chatToClients;
			sendPacket << type << text;		//And put along a type in a packet
			for (int j = 0; j < clients.size(); j++)
			{
				if (i != j)			//message will be sent to all the people apart from the one who sent it
				{
					clients[j]->send(sendPacket);
				}
			}
		}
		sendStartGame(clients[i]);		//game started confirmation is sent to everyone, to the ones who just connected too to allow them to move.
	}
}

void Server::UDP()
{
	receiveUDP();
}

void Server::receiveUDP()			//UDP received function.
{

	int id;
	int type;
	// UDP socket:
	sf::IpAddress sender;		//IP of sender
	sf::Packet receivePos;
	Player incomingClient;
	unsigned short port;
	while (UDP_socket.receive(receivePos, sender, port) == sf::Socket::Done)	//UDP received
	{
		receivePos >> type;		//Depending of the type of the packet, do different things
		receivePos >> id;		//Extract ID from packet.
		incomingClient.ip = sender;
		incomingClient.portUDP = port;
		incomingClient.clientID = id;
		bool match = false;
		for (int i = 0; i < Players.size(); i++)
		{
			if (Players[i].clientID == incomingClient.clientID)			//If the Id of the Player in the loop is the same as the Id of the client incoming, then there is a match
			{
				match = true;
			}
		}
		if (!match)
		{
			Players.push_back(incomingClient);			//If there is not a match, add the the incoming client to the vector of known players.e
		}

		if (type == udpPosReceived)		//if the type is positions, then send the position of the player to other players.
		{
			sendUDP(receivePos, id);
		}
		else if (type == disconnect)			//If type is 6, that person is disconnected
		{
			id_setter--;			//Id_setter variable decreases to avoid creating gaps
			std::cout << "ID: " << id_setter << " has disconnected. \n";
			std::cout << id_setter << "  is now a free id.\n ";
			clients.pop_back();			//Pop back a space in the vector, not ideal as can remove a player that shouldn't be removed.
			bool gameStarted = false;
			Players.pop_back();		//Pop back a space in the vector of players, not ideal as can remove a player that shouldn't be removed.
			std::cout << "There are other : " << clients.size() << " people in the server \n";
		}
		
	}
}


void Server::sendUDP(sf::Packet receivePosVar, int ID)			//Once you received positions, this is run
{
	int type = udpPosSent;	//Type of the packet for positions to be sent to other players.
	sf::Packet move;
	//int id_temp;
	float posX;		//Temp float to store positions received.
	float posY;		//Temp float to store positions received.
	//receivePosVar >> id_temp;
	receivePosVar >> posX;			//Positions received are stored (x,y)
	receivePosVar >> posY;
	sf::Packet sendToClients;		//Positions are sent back to all clients (they will be filtered on the client side not to use them on the player who sent them)
	sendToClients << type;			//information is put in the packet.
	sendToClients << ID;
	sendToClients << gameTime;
	sendToClients << posX;
	sendToClients << posY;

	for (int i = 0; i < Players.size(); i++)
	{
		//
		//		std::cout << "send " << i << std::endl;
		unsigned short port = Players[i].portUDP;
		sf::IpAddress address = Players[i].ip;
		if (UDP_socket.send(sendToClients, address, port) != sf::Socket::Done)		//Send everything to all the clients.
		{
			//	printf("message can't be sent\n");
		}
		else
		{
			//std::cout << "Positions of players have been successfully sent \n";
		}
	}
}

void Server::IdAndPositionSetter(sf::TcpSocket* sock, std::string name_)
{
	sf::Packet setupAsked;
	int type;

	if (sock->receive(setupAsked) == sf::Socket::Done)	//when the setup is asked
	{
		setupAsked >> type;
		if (type == idAsked)		//Send id
		{
			sf::Packet Id_And_Pos_Setter;
			int type = idSet;
			Id_And_Pos_Setter << type;
			Id_And_Pos_Setter << id_setter;
		}
	}

}

void Server::sendStartGame(sf::TcpSocket* sock)			//Once two players join, the game starts.
{
	sf::Time time1;			//Time to avoid sending always the start game and creating a queue of messages
	if (clients.size() >= 1)			//Game starts when one player connects.
	{
		time1 = startGameClock.getElapsedTime();
		sf::Packet startGame;			//Packet that holds start game confirmation
		int type = sendStartGameType;				//Type of start game packets
		startGame << type;
		if (time1.asSeconds() >= 0.2)			//Send the packet every 0.2 seconds
		{
			if (sock->send(startGame) != sf::Socket::Done)
			{
			}
			else
			{

				gameStarted = true;
			}
		}
	}
}

//void server::checkDisconnections(sf::TcpSocket* sock)			//Checks for disconnections every 3 seconds, function need to be changed, temporary
//{
//	sf::Packet discCheck;
//	sf::Time discTime;
//	discTime = discCheckClock.getElapsedTime();
//	if (discTime.asSeconds() >= 3)
//	{
//		if (sock->send(discCheck) != sf::Socket::Done)
//		{
//			id_setter--;
//			std::cout << "ID: " << id_setter << " has disconnected. \n";
//			std::cout << id_setter << "  is now a free id.\n ";
//			bool gameStarted = false;
//			howOftenSendGameTime.restart();
//			sock->disconnect();
//			std::cout << "There are other : " << clients.size() << " people in the server \n";
//		}
//		discCheckClock.restart();
//	}
//}

void Server::sendTime()		//SENDS TIME TO ALL CLIENTS, TIME WILL STARTS WHEN AT LEAST ONE PLAYER IS IN.
{

	if (gameStarted && clients.size() >= 1)		//Game starts and timer starts when at least one player is in.
	{
		if (gameStart == true)		//Clock restart when someone joins (clock starts when program is launched)
		{
			gameClock.restart();
			gameStart = false;
		}
		sf::Time gameSendTime = howOftenSendGameTime.getElapsedTime();
		int typeGameClock = sendTimeType;
		sf::Packet gameTimer;
		gameTime += gameClock.restart().asSeconds();
		//std::cout << gameTime << "\n";
		gameTimer << typeGameClock;
		gameTimer << gameTime;
		if (gameSendTime.asSeconds() >= 0.01)			//Send this packet with the game time every second
		{
			for (int i = 0; i < Players.size(); i++)
			{
				unsigned short port = Players[i].portUDP;
				sf::IpAddress address = Players[i].ip;
				if (UDP_socket.send(gameTimer, address, port) != sf::Socket::Done)
				{
					//	printf("message can't be sent\n");
				}
				else
				{
					//std::cout << "Positions of players have been successfully sent \n";
				}
			}
			howOftenSendGameTime.restart();		//Restart timer
		}
	}
}

void Server::BindUDP()
{
	// bind the socket to a port
	if (UDP_socket.bind(udp_port) != sf::Socket::Done)
	{
		printf("Error binding UDP\n");
	}
	else
	{
		printf("UDP bound correctly\n");
	}
}