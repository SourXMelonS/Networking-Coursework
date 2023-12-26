#include "Client.h"
Client::Client()
{

}
Client::Client(sf::IpAddress& ip, unsigned short& port, Player& p, std::string& name_, sf::TcpSocket* sock, sf::RenderWindow* window, Input* input_)
{
	socket = sock;
	Ip_serverAddress = ip;
	window_ = window;
	ID_And_Positions_Getter();
	Setup(&p);
	udp_port = 54000;
	name = name_;
	input = input_;
}

Client::~Client()
{

}

void Client::Update(Input* input_, sf::Event* Ev,Player* player_, float dt) //Main update loop
{

	deltaTime = dt;

	TCPReceive(); //Recieve TCP

	UDP_sendPosition(player_, input_, dt); //Send positions

	UDPReceive(player_); //Recieve Positions

	for (int i = 0; i < opponents.size(); i++) //Set Opponent positions
	{
		interpolateOpponentPos(&opponents.at(i), dt);
	}

	CheckCollision(player_); //Check for coin pickups

	disconnect(player_, input_); //Check if client has requested disconnect

	generateCoin(); //Generate coins

	timeText.setString("Game time: " + std::to_string(gameTime)); //Set strings
	scoreText.setString("Score: " + std::to_string(score));

}

void Client::HandleInput(sf::Event* Ev, Input* input, Player* p)
{
	if (input->isKeyDown(sf::Keyboard::T) && open_chat == false) //Chat opens		
	{
		input->setKeyUp(sf::Keyboard::T);
		open_chat = true;		
		chat_empty_on_open = true;		
	}
	else if (input->isKeyDown(sf::Keyboard::Escape) && open_chat == true) //Chat closes		 
	{
		input->setKeyUp(sf::Keyboard::Escape);
		if (userText.size() > 0)		
			userText.clear();
		open_chat = false;		
	}

	if (open_chat == true) //If the chat is open
	{
		is_chat_open = true; //Chat is open
		if (Ev->type == sf::Event::KeyPressed) //If a key is pressed
		{
			if (Ev->key.code == sf::Keyboard::Return) //Send message on enter
			{
				if (userText.size() > 0)
					sendMessageTCP(p);
				open_chat = false; //Chat closes once the user sends a message
			}

			else if (Ev->key.code == sf::Keyboard::BackSpace) 
			{
				if (userText.size() > 0)
					userText.pop_back();
			}
			else if (Ev->key.code == sf::Keyboard::Space) 
			{
				userText += ' ';
			}

		}
		else if (Ev->type == sf::Event::TextEntered) //User is adding text
		{
			if (32 < Ev->text.unicode && Ev->text.unicode < 128)
				userText += (char)Ev->text.unicode;
			//std::cout << "text is being entered\n\n";

			if (chat_empty_on_open == true)	
			{
				userText.pop_back();
				chat_empty_on_open = false;
			}
		}
	}
}

void Client::Render() //render the client 
{
	window_->draw(timeText); 
	window_->draw(scoreText);
	textSetup(window_);

	for (int i = 0; i < 20; i++)
	{
		coins[i].Render(window_);
	}
}

void Client::disconnect(Player* p, Input* input)
{
	sf::Packet temp;
	int id = id_getter;

	temp << sendDisconnection;
	temp << id;

	if (input->isKeyDown(sf::Keyboard::Escape)) //disconnect the player when they press escape and tell the server
	{
		if (udp_socket.send(temp, Ip_serverAddress, udp_port) != sf::Socket::Done)
		{
			std::cout << "Cannot send disconnect to server"<<std::endl;
		}
		else
		{
			std::cout << "Disconnect" << std::endl;
			window_->close();
		}
	}
}

void Client::Setup(Player* p)
{
	score = 0;
	font.loadFromFile("font/arial.ttf"); //Initialisation of font for text
	udp_socket.setBlocking(false); //Udp set to non blocking
	socket->setBlocking(false); //TCP set to non blocking
	open_chat = false;	
	connected_ = true; //Set client to connected
	chat_empty_on_open = false;	//Empties chat when t is pressed and the chat opens
	is_chat_open = false; //Boolean to know whether chat is open
	p->setPosition(player.playerStartPos.x, player.playerStartPos.y);
	RenderBackgound();
}

void Client::RenderBackgound() //Renders in text for the background
{
	timeText.setFont(font);
	timeText.setCharacterSize(50);
	timeText.setFillColor(Color::Black);
	timeText.setPosition(window_->getSize().x / 2, 100);
	timeText.setString("");

	scoreText.setFont(font);
	scoreText.setCharacterSize(45);
	scoreText.setFillColor(Color::Black);
	scoreText.setPosition(10, 100);
	scoreText.setString("");

}

void Client::sendMessageTCP(Player* p)
{
	sf::Text displayText("You " + userText, font, 15); 
	displayText.setFillColor(sf::Color::White);
	chat.push_back(displayText);
	sf::Packet packet;
	packet << name + ": " + userText;
	if (socket->send(packet) != sf::Socket::Done) //sends text chat
	{
		std::cout << "Error sending message. \n";
	}
	userText = "";
}

void Client::TCPReceive()
{
	sf::Packet startGame;
	while (socket->receive(startGame) == sf::Socket::Done) //recieves text chat
	{
		int type = 0;
		startGame >> type;
		
		if (type == chatReceived)
		{
			std::string temptext;
			if (startGame >> temptext)
			{
				sf::Text displayText(temptext, font, 15);
				displayText.setFillColor(sf::Color::Magenta);
				chat.push_back(displayText);
			}
		}
	}
}

void Client::ID_And_Positions_Getter()
{
	askSetup();
	sf::Packet Id_Getter;
	if (socket->receive(Id_Getter) != sf::Socket::Done) //retrieves initial player position and coin positions
	{
		std::cout << "Error getting ID\n";
	}
	else
	{
		int type;
		Id_Getter >> type;
		if (type == IdReceived)
		{
			Id_Getter >> id_getter;
			std::cout << "Your id is: " << id_getter << std::endl;
			Id_Getter >> player.playerStartPos.x >> player.playerStartPos.y;
		}
	}

	coinPosGetterTCP();
}

void Client::textSetup(sf::RenderWindow* window)
{
	//Render Messages
	int previousMessPos = drawText.getPosition().y - 25;
	window->draw(drawText);
	if (chat.size() > 0) //If chat is bigger than zero, a reverse iterator places them in a down to up position
	{
		std::vector<sf::Text>::reverse_iterator rev;
		int count = 0;
		for (rev = chat.rbegin(); rev != chat.rend(); ++rev)
		{
			rev->setPosition(0, previousMessPos - count * 25);
			++count;
			window->draw(*rev);
		}
	}
	if (chat.size() == 5) //Chat can't have more than five messages.
	{
		chat.erase(chat.begin());
	}
	drawText.setString(userText);
	drawText.setFont(font);
	drawText.setCharacterSize(15);
	drawText.setFillColor(sf::Color::Green);
	drawText.setPosition(0, window->getSize().y - 100);
}

void Client::UDP_sendPosition(Player* p, Input* input, float dt)
{
	sf::Time time1 = clock.getElapsedTime(); //Timer to keep track of how often positions are sent
	sf::Packet temp;
	temp << sendPlayerPos; //set type to playerpos
	float posX = p->getPosition().x;
	float posY = p->getPosition().y;
	if (open_chat == false)	//stops player from moving if chat is open
	{
		p->handleInput(input,dt);
	}
	if (time1.asSeconds() >= 0.01) //How often send the position of the player.
	{
		temp << id_getter << posX << posY;
		if (udp_socket.send(temp, Ip_serverAddress, udp_port) != sf::Socket::Done) //Try to send through socket.
		{
			printf("message can't be sent\n");
		}
		else if (time1.asSeconds() >= 0.01)
		{
		}
		clock.restart();
	}
}

void Client::UDPReceive(Player* p)
{
	sf::IpAddress sender;
	sf::Packet updated_pos;
	int opponentID = 0;
	int type = 0;

	unsigned short port = udp_port;
	while (udp_socket.receive(updated_pos, sender, port) == sf::Socket::Done)
	{
		updated_pos >> type;

		if (type == gameTimeReceive) //Updated game Time
		{
			updated_pos >> gameTime;
		}
		if (type == recieveOpponentPos)	//Recieves position
		{
			updated_pos >> opponentID;

			if (id_getter != opponentID) //If the id is not the client then update the opponents position
			{
				bool knowHim = false;
				for (int i = 0; i < opponents.size(); i++) {
					opponents.at(i).prevPos = opponents.at(i).getPosition();
					if (opponents.at(i).id == opponentID) //If opponent update their position
					{		 
						updated_pos >> opponents.at(i).tempT;
						updated_pos >> opponents.at(i).nextPos.x >> opponents.at(i).nextPos.y;		
						lerpAlpha = 0;			//Alpha is reset
						opponents.at(i).update(deltaTime);
						knowHim = true;
					}
				}
				if (!knowHim) 
				{ //If the opponent isnt known create new player
					Player temp;			
					temp.Init();			
					temp.setFillColor(sf::Color::Blue);
					temp.id = opponentID; //Set its id to the new one
					sf::Vector2f rec_pos;
					updated_pos >> rec_pos.x >> rec_pos.y; //Starting position of new player connected
					temp.setPosition(rec_pos);
					opponents.push_back(temp); //Add to opponent vector
					someoneJoined = true;
				}															
			}
		}
		if (type == coinHasBeenPicked) //update coin that has been picked
		{
			int coinNum = 0;
			updated_pos >> opponentID;
			updated_pos >> coinNum;
			coins_picked_up++;
			coins[coinNum].setPickedUp(true);
		}
		if (coins_picked_up > 14)
		{
			coinPosGetterUDP();
		}
	}
	
	
}
sf::Vector2f lerp(sf::Vector2f oldPos, sf::Vector2f currentPos, sf::Vector2f newPos, float alpha) //interpolation function
{
	//uses quadratic interpolation with the previous, current and next position
	//return ((1 - alpha) * oldPos + alpha * newPos);
	float PosX = ((oldPos.x * ((float)pow(1 - alpha, 2))) + (2.f * currentPos.x * alpha * (1 - alpha)) + (newPos.x * (float)pow(alpha, 2)));
	float PosY = ((oldPos.y * ((float)pow(1 - alpha, 2))) + (2.f * currentPos.y * alpha * (1 - alpha)) + (newPos.y * (float)pow(alpha, 2)));
	return Vector2f(PosX, PosY);
}

void Client::interpolateOpponentPos(Player* opponent, float dt)
{
	lerpAlpha = 0.5;		
	opponent->setPosition(lerp(opponent->prevPos,opponent->getPosition(), opponent->nextPos, lerpAlpha));
	
}

void Client::CheckCollision(Player* p)
{
	for (int i = 0; i < 20; i++)
	{
		if (coins[i].getPickedUp() == false)
		{
			if (Collision::checkBoundingBox(p, &coins[i]))
			{
				sf::Packet coinPickedPacket;
				int type = 8;
				coinPickedPacket << type << id_getter << i;
				if (udp_socket.send(coinPickedPacket, Ip_serverAddress, udp_port) == sf::Socket::Done)		//Check for collisions, if collision happens lets server know 
				{
					coins[i].setPickedUp(true);
					score++;
				}
			}
		}
	}
}

void Client::askSetup()
{
	int type;
	sf::Packet setupAsk;
	setupAsk << askForSetup;
	if (socket->send(setupAsk) != sf::Socket::Done) //Asks if the game has been setup
	{
		std::cout << "Error getting ID\n";
	}
}

void Client::generateCoin() //Function for checking amount of coins picked and reseting 
{
	int pickedUp = 0;
	for (int i = 0; i <= 20; i++)
	{
		if (coins[i].getPickedUp() == true) 
		{
			pickedUp++;
		}
	}
	if (pickedUp >= 15)
	{
		for (int i = 0; i < 20; i++)
		{
			coins[i].setPickedUp(false);
		}
	}

}
void Client::coinPosGetterTCP() //starting position to be recieved from server
{
	sf::Packet CoinposGetter; //Starting positions of coins
	if (socket->receive(CoinposGetter) != sf::Socket::Done)
	{
		std::cout << "Error getting ID\n";
	}
	else
	{
		int type = 0;
		CoinposGetter >> type;
		std::cout << type;
		if (type == coinPositions)
		{
			for (int i = 0; i < 20; i++) //Coin generation loop
			{
				CoinposGetter >> coinPos[i].x;
				CoinposGetter >> coinPos[i].y;
				std::cout << coinPos[i].x << "  -  " << coinPos[i].y << std::endl;
				coins[i].setPosition(coinPos[i].x, coinPos[i].y);
				coins[i].Update();
			}
		}
	}
}
void Client::coinPosGetterUDP() //attempt at further randomisation of coins every 15 picked
{
	sf::IpAddress sender;
	sf::Packet CoinposGetter;
	unsigned short port = udp_port;
	//Starting positions of coins
	if(udp_socket.receive(CoinposGetter,sender,port) != sf::Socket::Done)
	{
		//std::cout << "Error getting ID\n";
	}
	else
	{
		int type = 0;
		CoinposGetter >> type;
		std::cout << type;
		if (type == coinPositions)
		{
			for (int i = 0; i < 20; i++) //Coin generation loop
			{
				CoinposGetter >> coinPos[i].x;
				CoinposGetter >> coinPos[i].y;
				std::cout << coinPos[i].x << "  -  " << coinPos[i].y << std::endl;
				coins[i].setPosition(coinPos[i].x, coinPos[i].y);
				std::cout << coins[i].getPosition().x << "  -  " << coins[i].getPosition().y << std::endl;
				coins[i].Update();
			}
		}
		coins_picked_up = 0;
	}
}
bool Client::getConnectedStatus()
{
	return connected_;
}
