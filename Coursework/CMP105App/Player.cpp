#include "Player.h"
#include <iostream>
using namespace std;

Player::Player()
{
}

Player::~Player()
{
}

void Player::init(Input* input, RenderWindow* win)
{
	window = win;
	setInput(input);
	setWindow(win);

	txtr.loadFromFile("gfx/");

	setSize(Vector2f(32, 32));
	setPosition(Vector2f(100, 100));
	setVelocity(0, 0);
	collisionBox = FloatRect(0, 0, 16, 32);

	scale = 70.f;
	gravity = Vector2f(0, 9.8f);
}

void Player::handleInput(float dt)
{
	if (input->isKeyDown(sf::Keyboard::W))
	{
		if(velocity.y <= 200)
			velocity.y +=5;
	}
	else
	{
		if (velocity.y > 0)
			velocity.y -= 10 - (2 * (velocity.y / 100));
	}
}

void Player::update(float dt)
{
	//velocity += gravity * dt * scale / 2.f;

	move(velocity * dt);
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

void Player::render()
{
}
