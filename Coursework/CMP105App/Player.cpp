#include "Player.h"
#include <iostream>
using namespace std;

Player::Player()
{
}

Player::~Player()
{
}

//void Player::init(Input* input, RenderWindow* win)
//{
//	window = win;
//	setInput(input);
//	setWindow(win);

	//txtr.loadFromFile("gfx/");

//	srand(time(NULL));
	
//
//}

void Player::Init()
{
	playerBody.setSize(Vector2f(16, 32));
	playerBody.setPosition(Vector2f(100, 100));
	
	playerBody.setOutlineThickness(2);
	playerBody.setOutlineColor(sf::Color(0, 0, 0, 255));
	playerBody.setFillColor(sf::Color(255, 0, 0, 255));
	setCollisionBox(0, 0, 16, 32);
	srand(time(NULL));
}

void Player::handleInput(Input* input,float dt)
{
	if (input->isKeyDown(sf::Keyboard::A))
	{
		move(-20 * dt, 0);
	}
	if (input->isKeyDown(sf::Keyboard::D))
	{
		move(20 * dt, 0);
		
	}
	if (input->isKeyDown(sf::Keyboard::W))
	{
		move(0, -20 * dt);
	}
	if (input->isKeyDown(sf::Keyboard::S))
	{
		move(0, 20 * dt);
	}
	else
	{
		velocity = sf::Vector2f(0, 0);
	}
}

void Player::update(float dt)
{
	//velocity += gravity * dt * scale / 2.f;
	playerBody.setPosition(getPosition().x + 4, getPosition().y + 4);
	//move(velocity * dt);
}

void Player::collisionResponce(GameObject* collider)
{
	if (Collision::TouchTopof(this, collider)) {
		velocity.y = 0;
		//Top of tile
		setPosition(Vector2f(getPosition().x, collider->getCollisionBox().top - collisionBox.height));
	
	}
	else if (Collision::TouchBottomof(this, collider)) {
		velocity.y = 0;
		//Bottom of tile
		setPosition(Vector2f(getPosition().x, collider->getCollisionBox().top + collider->getCollisionBox().height + collisionBox.height));
	}
	else if (Collision::TouchLeftof(this, collider)) {
		velocity.x = 0;
		//Left side of tile
		setPosition(collider->getCollisionBox().left - collisionBox.width, getPosition().y);
	}
	else if (Collision::TouchRightof(this, collider)) {
		velocity.x = 0;
		//Right side of tile
		setPosition(collider->getCollisionBox().left + collider->getCollisionBox().width, getPosition().y);
	}
}

FloatRect Player::getCollisionBox()
{
	return collisionBox;
}

void Player::render(sf::RenderWindow* window_)
{
	window_->draw(playerBody);
}
