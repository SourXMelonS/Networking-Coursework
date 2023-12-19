// Main
// Entry point of the application.
// Handles window creation and window events.
// Contains the game loop, updates input class and contains the level objects.
// @author Paul Robertson

#include <iostream>
#include "Level.h"
#include "Framework/AudioManager.h"
#include "Framework/GameState.h"
#include "Menu.h"
#include "Client.h"


void windowProcess(sf::RenderWindow* window, Input* in)
{
	
}

int main()
{
	//Create the window
	sf::RenderWindow window(sf::VideoMode(1200, 675), "CMP303_Racing_Game");
	sf::TcpSocket Tcp;

	// Initialise input and level objects.
	AudioManager audioManager;
	Input input;
	GameState gameState;


	Level level(&window, &input, &gameState, &audioManager, &Tcp);
	//level = new Level(&window, &input, &gameState, &audioManager);
	Menu menu(&window, &input, &gameState, &Tcp);

	bool levelInit = false;
	bool menuInitialised = false;
	// Initialise objects for delta time

	sf::Clock clock;
	float deltaTime;
	gameState.setCurrentState(State::MENU);
	// Game Loop
	while (window.isOpen())
	{
		deltaTime = clock.restart().asSeconds() * 2;
		// Handle window events.
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::Resized:
				window.setView(sf::View(sf::FloatRect(0.f, 0.f, (float)event.size.width, (float)event.size.height)));
				break;
			case sf::Event::KeyPressed:
				// update input class
				input.setKeyDown(event.key.code);
				break;
			case sf::Event::KeyReleased:
				//update input class
				input.setKeyUp(event.key.code);
				break;
			case sf::Event::MouseMoved:
				//update input class
				input.setMousePosition(event.mouseMove.x, event.mouseMove.y);
				break;
			case sf::Event::MouseButtonPressed:
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					//update input class
					input.setLeftMouse(Input::MouseState::PRESSED);
				}
				else if (event.mouseButton.button == sf::Mouse::Right)
				{
					input.setRightMouse(Input::MouseState::PRESSED);
				}
				break;
			case sf::Event::MouseButtonReleased:
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					//update input class
					input.setLeftMouse(Input::MouseState::UP);
				}
				else if (event.mouseButton.button == sf::Mouse::Right)
				{
					input.setRightMouse(Input::MouseState::UP);
				}
				break;
			default:
				// don't handle other events
				break;
			}
			if (gameState.getCurrentState() == State::LEVEL)
			{
				if (level.getConnectedStatus() == false)
				{
					level.Init(&menu);
				}
				level.clientInput(&event);
			}
			if (gameState.getCurrentState() == State::MENU)
			{
				if (menuInitialised == false)
				{
					menu.Init();
					menuInitialised = true;
				}
				menu.handleInput(&event);
			}
			
		}
		
		
		// Call standard game loop functions (input, update and render)
		switch (gameState.getCurrentState())
		{
		case(State::MENU):
			menu.update(deltaTime);
			menu.render();
			break;
		case(State::LEVEL):
			level.handleInput(deltaTime);
			level.update(deltaTime, &event);
			level.render();
			break;
		}
		

		// Update input class, handle pressed keys
		// Must be done last.
		//input.update();
	}

}
