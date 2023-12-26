#include "server.h"

void server::Init()
{
	std::cout << "The local server address is: \n" << sf::IpAddress::getLocalAddress()<<"\n";			
	std::cout << "The public server address is: \n" << sf::IpAddress::getPublicAddress() <<"\n";		
	UDP_port= 54000; //UDP port hardcoded
	listener.listen(53000);	//TCP port hardcoded
	listener.setBlocking(false); //Listener doesn't block so that it doesn't stop events from happening 
	selector.add(listener);	//Selector adds listener
	UDP_socket.setBlocking(false);	//UDP socket set to not blocking, 
	srand(time(NULL));
	BindUDP();			
}

void server::Update()
{
	
	bool done = false;
	while (!done) //infinite loop
	{
		TCP();			
		UDP();			
		sendTime();	//after UDP and TCP, send the game time to everyone.
	}
	
}

void server::TCP()
{
	TCPCommunicationHandler();
}

void server::TCPCommunicationHandler()
{

	sf::TcpSocket* socket = new sf::TcpSocket;
	if (listener.accept(*socket) != sf::Socket::Done) //accepted a new connection
	{
		//std::cout << "error accepting new conn\n";
	}
	else
	{
		if (clients.size() < 20) //maximum server size 
		{
			selector.add(*socket); //If connection is accepted, add the socket to the socket list
			sf::Packet name_packet;		
			std::string name;
			if (socket->receive(name_packet) == sf::Socket::Done) //If socket sends the name store it to display who connected to the server
			{
				int type =0;
				name_packet >> type;
				if (type == namePacket) //Packet type is 1
				{
					name_packet >> name;
					std::cout << name << " has connected to the chat room. " << std::endl; //display who connected to server
				}
				IdAndPositionSetter(socket, name); //when someone connects, set its positions and id
			}

			socket->setBlocking(false); //Each socket is not blocking
			clients.push_back(socket); //Socket is added to the clients vector
			connected = true;	
		}
	}
	TCPMessageRecSend();			
}

void server::TCPMessageRecSend() //Handles TCP events
{
	for (int i = 0; i < clients.size(); i++)
	{
			sf::Packet packet, sendPacket;
			if (clients[i]->receive(packet) == sf::Socket::Done) //If a message is received
			{
				std::cout << "someone sent a message\n"; //Display to server
				std::string text;
				packet >>text; //Message is extracted
				int type = chatToClients;
				sendPacket <<type<<  text;
				for (int j = 0; j < clients.size(); j++)
				{
					if (i != j)	//message will be sent to all the people apart from the one who sent it
					{
						clients[j]->send(sendPacket);
					}
				}
			}
			sendStartGame(clients[i]); //game started confirmation is sent to everyone
	}
}

void server::UDP()
{
	receiveUDP();
}

void server::receiveUDP() //UDP received function.
{

	int id;
	int type;
	// UDP socket:
	sf::IpAddress sender; //IP of sender
	sf::Packet receivePos;
	Player incomingClient;
	unsigned short port;
	while (UDP_socket.receive(receivePos, sender, port) == sf::Socket::Done) //UDP received
	{
		receivePos >> type;	//type of packet is stored to be sorted later
		receivePos >> id;		
		incomingClient.ip = sender;
		incomingClient.portUDP = port;
		incomingClient.clientID = id;
		bool match = false;
		for (int i = 0; i < Players.size(); i++)		
		{
			if (Players[i].clientID == incomingClient.clientID)	//If the Id of the Player in the loop is the same as the Id of the client incoming, then there is a match
			{
				match = true;
			}
		}
		if (!match)
		{
			Players.push_back(incomingClient);	//If there is not a match, add the the incoming client to the vector of known players.e
		}

		if (type == udpPosReceived)	//if the type is positions, then send the position of the player to other players.
		{
			sendUDP(receivePos, id);
		}
		else if (type == disconnect) //If type is disconnect then disconnect them
		{
		id_setter--;
		std::cout << "ID: " << id_setter << " has disconnected. \n";
		std::cout << id_setter << "  is now a free id.\n ";
		clients.pop_back();	
		bool gameStarted = false;
		Players.pop_back();		
		std::cout << "There are other : " << clients.size() << " people in the server \n";
		}
		else if (type == coinPicked)
		{
			
			coinPickedEvent(receivePos, id); //If someone picked a coin.
		}
	}
}


void server::sendUDP(sf::Packet receivePosVar,int ID) //Return positions to clients
{
		int type = udpPosSent;	//Type of the packet for positions to be sent to other players.
		sf::Packet move;
		float posX;		
		float posY;		
		receivePosVar >> posX; //Positions received are stored
		receivePosVar >> posY;
		sf::Packet sendToClients; //Positions are sent back to all clients 
		sendToClients << type; //information is put in the packet.
		sendToClients << ID;
		sendToClients << gameTime;
		sendToClients << posX;
		sendToClients << posY;

		for (int i = 0; i < Players.size(); i++)
		{
	//
			//std::cout << "send " << i << std::endl;
			unsigned short port = Players[i].portUDP;
			sf::IpAddress address = Players[i].ip;
			if (UDP_socket.send(sendToClients, address, port) != sf::Socket::Done) //Send everything to all the clients.
			{
				printf("message can't be sent\n");
			}
			else
			{
				//std::cout << "Positions of players have been successfully sent \n";
			}
		}
}

void server::coinPickedEvent(sf::Packet pack, int id) //Called when a coin is picked
{
	int coinNum = 0;
	int type = coinPickedsendToClients;
	pack >> coinNum; //Sent the coin that has been collected
	coins_picked++;
	std::cout << id << " collected coin:" << coinNum << "\n coins picked: "<<coins_picked<<std::endl; //Display who picked up a coin
	sf::Packet coin_Picked;		
	coin_Picked << type; //send coin picked to all the players
	coin_Picked << id;
	coin_Picked << coinNum;
	for (int i = 0; i < Players.size(); i++)
	{
		unsigned short port = Players[i].portUDP;
		sf::IpAddress address = Players[i].ip;
		if (UDP_socket.send(coin_Picked, address, port) != sf::Socket::Done)
		{
			printf("message can't be sent in coin picked event\n");
		}
		else
		{
			
			if (coins_picked > 15)
			{
				
				CoinRandomise();
				if (UDP_socket.send(coinPositionPacket, address, port) != sf::Socket::Done)	//Send positions of coins to clients.
				{
					std::cout << "Error sending message. \n";
				}
				else
				{

				}
				coins_picked = 0;
			}
		}
	}
}

void server::IdAndPositionSetter(sf::TcpSocket* sock, std::string name_) //Called when new connections/player connected
{	
	sf::Packet setupAsked;
	int type;
	
	if (sock->receive(setupAsked) == sf::Socket::Done) //when the setup is asked
	{
		setupAsked >> type;
		if (type == idAsked) //Send id
		{
			sf::Packet Id_And_Pos_Setter;
			int type = idSet;
			Id_And_Pos_Setter << type;
			Id_And_Pos_Setter << id_setter;


			for (int i = 0; i < Players.size(); i++) //Positions are created randomly
			{
				Players[i].startPos.x = static_cast <float> (rand() % 1000) + 100;
				Players[i].startPos.y = static_cast <float> (rand() % 475) + 100;
				Id_And_Pos_Setter << Players[i].startPos.x << Players[i].startPos.y; //Puts position into packet
			}


			if (sock->send(Id_And_Pos_Setter) != sf::Socket::Done)	//Send starting positions and id
			{
				std::cout << "Error sending message. \n";
			}
			else
			{
				std::cout << name_ << " is id: " << id_setter << "\n";
				id_setter++;
			}

			CoinRandomise();

			if (sock->send(coinPositionPacket) != sf::Socket::Done)	//Send positions of coins to clients.
			{
				std::cout << "Error sending message. \n";
			}
			else
			{

			}

		}
	}
	
}

void server::CoinRandomise()
{
	srand(time(NULL));
	int type1 = coinGen;
	coinPositionPacket << type1;
	if (generationDone == false || coins_picked >= 15)	//Generates all coin locations at random to be sent to the clients
	{
		for (int i = 0; i < 20; i++)
		{
			float x = static_cast <float> (rand() % 1200);
			float y = static_cast <float> (rand() % 675);
			coinPos[i].x = x;
			coinPos[i].y = y;
			coinPositionPacket << coinPos[i].x;
			coinPositionPacket << coinPos[i].y;
			std::cout << coinPos[i].x << "  -  " << coinPos[i].y << std::endl;
		}
		generationDone = true;		
	}
}



void server::sendStartGame(sf::TcpSocket* sock)	
{
	sf::Time time1;			
	if (clients.size() >= 1) //Game starts when one player connects.
	{
		time1 = startGameClock.getElapsedTime();
		sf::Packet startGame; //Packet that holds start game confirmation
		int type = sendStartGameType;
		startGame << type;
		if (time1.asSeconds() >= 0.2) //Send the packet every 0.2 seconds
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


void server::sendTime()	
{
	
	if (gameStarted&&clients.size()>=1)	//Game starts and timer starts when at least one player is in.
	{
		if (gameStart == true)	
		{
			gameClock.restart();
			gameStart = false;
		}
		sf::Time gameSendTime = sendGameTime.getElapsedTime();	
		int typeGameClock = sendTimeType;
		sf::Packet gameTimer;
		gameTime += gameClock.restart().asSeconds();
		//std::cout << gameTime << "\n";
		gameTimer << typeGameClock;
		gameTimer << gameTime;
		if (gameSendTime.asSeconds() >= 0.01) //Sends packet with the game time
		{
			for (int i = 0; i < Players.size(); i++)
			{
				unsigned short port = Players[i].portUDP;
				sf::IpAddress address = Players[i].ip;
				if (UDP_socket.send(gameTimer, address, port) != sf::Socket::Done)
				{
						printf("message can't be sent from send time\n");
				}
				else
				{
					//std::cout << "Positions of players have been successfully sent \n";
				}
			}
			sendGameTime.restart();	//Restart timer
		}
	}
}

void server::BindUDP()
{
	// bind the socket to a port
	if (UDP_socket.bind(UDP_port) != sf::Socket::Done)
	{
		printf("Error binding UDP\n");
	}
	else
	{
		printf("UDP bound correctly\n");
	}
}
