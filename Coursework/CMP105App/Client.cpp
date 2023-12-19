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
	canMove = true;
	input = input_;
}

Client::~Client()
{

}

void Client::Update(Input* input_, sf::Event* Ev,Player* player_, float dt)
{

	deltaTime = dt;

	TCPReceive();

	UDP_sendPosition(player_, input_, dt);

	UDPReceive(player_);

	for (int i = 0; i < opponents.size(); i++)
	{
		interpolateOpponentPos(&opponents.at(i), dt);
	}

	CheckCollision(player_);

	disconnect(player_, input_);

}

/*void Client::Init()
{
	sf::IpAddress ip_;
	ip_ = ip_.getLocalAddress();
	sf::TcpSocket socket;
	bool done = false;
	std::string id;
	std::string text = "";

	std::cout << "enter name: ";
	std::cin >> id;

	socket.connect(ip_, 10);
	sf::RenderWindow window(sf::VideoMode(800, 600, 32), "Chat");
	std::vector <sf::Text>chat;

	sf::Packet packet;
	packet << id;
	socket.send(packet);
	socket.setBlocking(false);
	window.setTitle(id);

	sf::Font font;
	font.loadFromFile("arial.ttf");
	while (!done)
	{
		sf::Event Event;
		while (window.pollEvent(Event))
		{
			switch (Event.type)
			{
			case sf::Event::Closed:
				window.close();
			case::sf::Event::KeyPressed:
				if (Event.key.code == sf::Keyboard::Escape)
					window.close();
				else if (Event.key.code == sf::Keyboard::Return)
				{
					sf::Packet packet;
					packet << id + ": " + text;
					socket.send(packet);
					sf::Text displayText(text, font, 20);
					displayText.setFillColor(sf::Color::Red);
					chat.push_back(displayText);
					text = "";
				}
				break;
			case sf::Event::TextEntered:
				text += Event.text.unicode;
				//	break;
			}
		}
		sf::Packet packet;
		socket.receive(packet);
		std::string temptext;
		if (packet >> temptext)
		{
			sf::Text displayText(temptext, font, 20);
			displayText.setFillColor(sf::Color::Blue);
			chat.push_back(displayText);
		}
		int i = 0;
		for (i; i < chat.size(); i++)
		{
			chat[i].setPosition(0, i * 20);
			window.draw(chat[i]);
		}
		sf::Text drawText(text, font, 20);
		drawText.setFillColor(sf::Color::Red);
		drawText.setPosition(0, i * 20);
		window.draw(drawText);
		//std::cout << "text is being rendered";
		window.display();
		window.clear();
	}

}
*/

void Client::HandleInput(sf::Event* Ev, Input* input, Player* p)
{
	if (input->isKeyDown(sf::Keyboard::T) && open_chat == false)		
	{
		//Chat opens
		std::cout << "Chat should be open\n";
		input->setKeyUp(sf::Keyboard::T);
		open_chat = true;		
		chat_empty_on_open = true;		
	}
	else if (input->isKeyDown(sf::Keyboard::Escape) && open_chat == true)		 
	{
		//Chat closes
		std::cout << "Chat should close and be emptied\n";
		input->setKeyUp(sf::Keyboard::Escape);
		if (userText.size() > 0)		
			userText.clear();
		open_chat = false;		
	}

	if (open_chat == true) //If the chat is open
	{
		is_chat_open = true; //Chat is open
		if (Ev->type == sf::Event::KeyPressed)	//If a key is pressed
		{
			if (Ev->key.code == sf::Keyboard::Return) //Send message on enter
			{
				if (userText.size() > 0)
					sendMessageTCP(p);
				open_chat = false;	//Chat closes once the user sends a message
			}

			else if (Ev->key.code == sf::Keyboard::BackSpace) //Removes last letter in the message in the chat
			{
				if (userText.size() > 0)
					userText.pop_back();
			}
			else if (Ev->key.code == sf::Keyboard::Space) //Space added
			{
				userText += ' ';
			}

		}
		else if (Ev->type == sf::Event::TextEntered)		//Text is being entered
		{
			if (32 < Ev->text.unicode && Ev->text.unicode < 128)
				userText += (char)Ev->text.unicode;
			std::cout << "text is being entered\n\n";

			if (chat_empty_on_open == true)			//If on chat opening the chat has message ('t') chat is emptied
			{
				userText.pop_back();
				chat_empty_on_open = false;
			}
		}
	}
}

void Client::Render()
{
	textSetup(window_);
	for (int i = 0; i <= 10; i++)
	{
		coins[i].Render(window_);
	}
}

void Client::disconnect(Player* p, Input* input)
{
}

void Client::Setup(Player* p)
{
	font.loadFromFile("font/arial.ttf");			//Initialisation of different texts.
	udp_socket.setBlocking(false);			//Udp set to non blocking. (it doesn't wait for an event to happen.)
	socket->setBlocking(false);			//TCP set to non blocking.
	open_chat = false;			//Know whether chat is open
	connected_ = true;		//Know whether client is connected
	chat_empty_on_open = false;			//Empties chat when t is pressed and the chat opens
	is_chat_open = false;		//Variable to know whether chat is open
	speed = 150;
	p->setPosition(player.playerStartPos.x, player.playerStartPos.y);
}

void Client::sendMessageTCP(Player* p)
{
	sf::Text displayText("You " + userText, font, 15);
	displayText.setFillColor(sf::Color::White);
	chat.push_back(displayText);
	sf::Packet packet;
	packet << name + ": " + userText;
	if (socket->send(packet) != sf::Socket::Done)
	{
		std::cout << "Error sending message. \n";
	}
	userText = "";
}

void Client::TCPReceive()
{
	sf::Packet startGame;
	while (socket->receive(startGame) == sf::Socket::Done)
	{
		int type = 0;
		startGame >> type;
		if (type == startGameType)
		{
			render_preStart = false;
			renderGameStartedElements = true;
		}
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
	if (socket->receive(Id_Getter) != sf::Socket::Done)
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
}

void Client::textSetup(sf::RenderWindow* window)
{
	//Render Messages
	int previousMessPos = drawText.getPosition().y - 25;
	window->draw(drawText);
	if (chat.size() > 0)		//If chat is bigger than zero, a reverse iterator places them in a down to up position
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
	if (chat.size() == 5)			//Chat can't have more than five messages.
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
	sf::Time time1 = clock.getElapsedTime();			//Timer to keep track of how often we send the positions
	sf::Packet temp;
	temp << sendPlayerPos;		//Type of UDP packet is three
	float posX = p->getPosition().x;
	float posY = p->getPosition().y;
	if (open_chat == false)	//prevents player from moving if chat is open
	{
		p->handleInput(input,dt);
	}
	if (time1.asSeconds() >= 0.01)		//How often send the position of the player.
	{
		temp << id_getter << posX << posY;
		if (udp_socket.send(temp, Ip_serverAddress, udp_port) != sf::Socket::Done)			//Send to the UDP socket.
		{
			printf("message can't be sent\n");
		}
		else if (time1.asSeconds() >= 0.01)
		{
		}
		clock.restart();			//Restart clock.
	}
}

void Client::UDPReceive(Player* p)
{
	sf::IpAddress sender;
	sf::Packet updated_pos;
	int opponentID;
	int type = 0;

	unsigned short port = udp_port;
	while (udp_socket.receive(updated_pos, sender, port) == sf::Socket::Done)
	{
		updated_pos >> type;
		if (type == gameTimeReceive)			//Updated game Time
		{
			updated_pos >> gameTime;
		}
	}
	if (type == recieveOpponentPos)		//RECEIVES UPDATED ENEMY POSITIONS
	{
		updated_pos >> opponentID;

		if (id_getter != opponentID)			//IF THE ID RECEIVED IS NOT THE MAIN PLAYER ONE, THEN UPDATE OTHER PLAYERS POSTIONS
		{
			bool knowHim = false;
			for (int i = 0; i < opponents.size(); i++) {
				if (opponents.at(i).id == opponentID) {		//IF ENEMY 
					updated_pos >> opponents.at(i).tempT;			//TIME RECEIVED FROM PLAYERS POSITIONS
					updated_pos >> opponents.at(i).nextPos.x >> opponents.at(i).nextPos.y;		//Updated enemy position for each enemy
					//enemies.at(i).setPosition(enemies.at(i).next_pos.x, enemies.at(i).next_pos.y);		//SET POSITION TO NEW POSITION, CHANGE FOR INTERPOLATION
					lerpAlpha = 0;			//Alpha is reset
					opponents.at(i).update(deltaTime);
					knowHim = true;
				}
			}
			if (!knowHim) {		//If the enemy is not known
				Player temp;			//Create a temporary player
				temp.Init();			//Initialise him
				temp.setFillColor(sf::Color::Blue);
				temp.id = opponentID;			//Set its id to the new one
				sf::Vector2f rec_pos;
				updated_pos >> rec_pos.x >> rec_pos.y;			//Starting position of new player connected
				temp.setPosition(rec_pos);
				opponents.push_back(temp);			//Add him to enemy vector
				someoneJoined = true;
			}															//	m_Messages.push_back(Player2.next_pos);					//Message history for prediction.
		}
	}
	if (type == coinHasBeenPicked)				//SOMEONE PICKED A COIN
	{
		int coinNum;
		updated_pos >> opponentID;
		updated_pos >> coinNum;
		
		coins[coinNum].setPickedUp(true);
	}
}
sf::Vector2f lerp(sf::Vector2f oldPos, sf::Vector2f newPos, float alpha)		//interpolation function
{
	return ((1 - alpha) * oldPos + alpha * newPos);

}

void Client::interpolateOpponentPos(Player* opponent, float dt)
{
	lerpAlpha += dt;		
	//std::cout << opponent->getPosition().x << "   " << opponent->getPosition().y << "NEWPOS: " << opponent->nextPos.x << "    " << opponent->nextPos.y << "\n";
	opponent->setPosition(lerp(opponent->getPosition(), opponent->nextPos, lerpAlpha));
}

void Client::CheckCollision(Player* p)
{
	
}

void Client::askSetup()
{
	int type;
	sf::Packet setupAsk;
	setupAsk << askForSetup;
	if (socket->send(setupAsk) != sf::Socket::Done)
	{
		std::cout << "Error getting ID\n";
	}
}

void Client::generateCoin(sf::Vector2f coinPos_[20])
{
	Coins coin;
	coin.setPosition(coinPos_->x, coinPos_->y);
	coin.Update();
}
void Client::coinPosGetter()
{
	sf::Packet CoinposGetter;			//Starting positions of coins
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
			for (int i = 0; i < 20; i++)			//Coin generation loop
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

bool Client::getConnectedStatus()
{
	return connected_;
}
