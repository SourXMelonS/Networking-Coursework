#include "MenuButton.h"

MenuButton::MenuButton()
{
}

MenuButton::MenuButton(Texture& txtr, Input* in, RenderWindow* window, int choice)
{
	setWindow(window);
	setInput(in);
	setSize(Vector2f(147, 60));
	setCollisionBox(0, 0, getSize().x, getSize().y);
	setTexture(&txtr);

	//Depending on the choice a different texture and positon will be set to the button
	if (choice == 1)
	{
		textureHover.loadFromFile("gfx/PlayCol_Button.png");
		setPosition(Vector2f(((window->getPosition().x * 1.6) - (getSize().x / 2)), 460));
	}
	else if (choice == 2)
	{
		textureHover.loadFromFile("gfx/ExitCol_Button.png");
		setPosition(Vector2f(((window->getPosition().x * 1.6) - (getSize().x / 2)), 525));
	}
	if (choice == 3)
	{
		textureHover.loadFromFile("gfx/Resumecol_Button.png");
		setPosition(Vector2f(((window->getPosition().x * 1.6) - (getSize().x / 2)), 460));
	}
	if (choice == 4)
	{
		textureHover.loadFromFile("gfx/Newgamecol_Button.png");
		setPosition(Vector2f(((window->getPosition().x * 1.6) - (getSize().x / 2)), 395));
	}

}

MenuButton::~MenuButton()
{
}

void MenuButton::handleInput(float dt)
{
}

void MenuButton::collisionResponse(GameObject*)
{
	//setting the texture for when the mouse collides with the button
	setTexture(&textureHover);
}

void MenuButton::changePosition(Vector2f newPosition) {
	//changing the position of the button
	setPosition(newPosition);
}