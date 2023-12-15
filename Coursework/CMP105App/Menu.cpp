#include "Menu.h"

Menu::Menu(RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud)
{
	window = hwnd;
	input = in;
	audio = aud;
	gameState = gs;

	backgroundTexture.loadFromFile("gfx/....png");
	hostTexture.loadFromFile("gfx/....png");
	joinTexture.loadFromFile("gfx/....png");

	menuBackground.setSize(Vector2f(window->getSize().x, window->getSize().y));
	menuBackground.setTexture(&backgroundTexture);

	hostButton = MenuButton(hostTexture, input, window, 1);
	joinButton = MenuButton(joinTexture, input, window, 2);


	font.loadFromFile("font/arial.ttf");

	if (!font.loadFromFile("font/arial.ttf"))
	{
		std::cout << "Font not found" << std::endl;
	}

	gameName.setFont(font);
	gameName.setCharacterSize(150);
	gameName.setFillColor(Color::Color(255, 176, 102));
	gameName.setPosition(window->getSize().x / 2 - 350, 100);

	gameName.setString("Generic Racing Game");
}
Menu::~Menu()
{


}

void Menu::handleInput(float dt)
{
	if (Collision::checkBoundingBox(&hostButton, (Vector2i(input->getMouseX(), input->getMouseY()))) && (input->isLeftMouseDown()))
	{
		audio->stopAllMusic();
		gameState->setCurrentState(State::HOST);
	}
	if (Collision::checkBoundingBox(&joinButton, (Vector2i(input->getMouseX(), input->getMouseY()))) && (input->isLeftMouseDown()))
	{
		audio->stopAllMusic();
		gameState->setCurrentState(State::JOIN);
	}
	//If the player clicks on exit the game closes
	if (Collision::checkBoundingBox(&exitButton, (Vector2i(input->getMouseX(), input->getMouseY()))) && (input->isLeftMouseDown()))
	{
		window->close();
	}
}

void Menu::update(float dt)
{
	if (Collision::checkBoundingBox(&hostButton, (Vector2i(input->getMouseX(), input->getMouseY())))) {
		hostButton.collisionResponse(NULL);
		if (!playedButton1)
		{
			playedButton1 = true;
		}
	}
	else
	{
		hostButton.setTexture(&hostTexture);
		playedButton1 = false;
	}
	if (Collision::checkBoundingBox(&joinButton, (Vector2i(input->getMouseX(), input->getMouseY()))))
	{
		joinButton.collisionResponse(NULL);
		if (!playedButton2)
		{
			playedButton2 = true;
		}
	}
	else
	{
		joinButton.setTexture(&joinTexture);
		playedButton2 = false;
	}

	if (Collision::checkBoundingBox(&exitButton, (Vector2i(input->getMouseX(), input->getMouseY())))) {
		exitButton.collisionResponse(NULL);
		if (!playedButton3)
		{
			playedButton3 = true;
		}
	}
	else
	{
		exitButton.setTexture(&exitTexture);
		playedButton3 = false;
	}
}
void Menu::render()
{
	//Game title
	
	
	beginDraw();
	window->draw(menuBackground);
	window->draw(gameName);
	window->draw(hostButton);
	window->draw(joinButton);
	window->draw(exitButton);
	endDraw();
}