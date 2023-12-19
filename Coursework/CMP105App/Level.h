#pragma once

#include <SFML/Graphics.hpp>
#include "Framework/BaseLevel.h"
#include "Player.h"
#include "Client.h"
#include "Menu.h"
#include <string>
#include <iostream>

class Level : BaseLevel{
public:
	
	Level(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud,sf::TcpSocket* sock);
	~Level();

	void handleInput(float dt) override;
	void update(float dt)override;
	void update(float dt, sf::Event*  event_);
	void clientInput(sf::Event* event_);
	void render();
	void Init(Menu* menu_);

	bool getConnectedStatus() { return connected_succesfully; };
private:
	// Default functions for rendering to the screen.

	bool connected_succesfully;
	std::unique_ptr<Client> client_;

	Player player;
	std::vector<Player> opponents;
	Menu* menu;

	unsigned short port;
	sf::IpAddress IP_ADDRESS;
	std::string name;
	sf::TcpSocket* Tcp;

	int speed;
	Font font;
	Text controls, speedText;
	View gameView;
	// Default variables for level class.
	
};