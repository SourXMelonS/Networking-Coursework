#include "Client.h"
Client::Client()
{

}
Client::Client(sf::IpAddress& ip, unsigned short& port, Player& p, std::string& name_, sf::TcpSocket* sock, sf::RenderWindow* window)
{
	socket = sock;
	Ip_serverAddress = ip;
	window_ = window;
	ID_And_Positions_Getter();
	Setup(&p);
	udp_port = 54000;
	name = name_;
	canMove = true;
}

Client::~Client()
{

}

void Client::Update(Input* input_, sf::Event* Ev,Player* player_, float dt)
{
	TCPReceive();

	UDP_sendPosition(player_, input_, dt);

	UDPReceive(player_);

	CheckCollision(player_);

	disconnect(player_, input_);
}

void Client::Init()
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

void Client::HandleInput(sf::Event* Ev, Input* input, Player* p)
{
	if (input->isKeyDown(sf::Keyboard::Enter) && open_chat == false)		
	{
		//Chat opens
		std::cout << "Chat should be open\n";
		input->setKeyUp(sf::Keyboard::Enter);
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
}

void Client::disconnect(Player* p, Input* input)
{
}

void Client::Setup(Player* p)
{
}

void Client::sendMessageTCP(Player* p)
{
}

void Client::TCPReceive()
{
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
			Id_Getter >> Player1.playerStartPos.x >> Player1.playerStartPos.y;
		}
	}
}

void Client::textSetup(sf::RenderWindow* window)
{
}

void Client::UDP_sendPosition(Player* p, Input* input, float dt)
{
}

void Client::UDPReceive(Player* p)
{
}

void Client::interpolateEnemyPos(Player* Player, float dt)
{
}

void Client::CheckCollision(Player* p)
{
}

void Client::askSetup()
{
}

bool Client::getConnectedStatus()
{
	return false;
}
