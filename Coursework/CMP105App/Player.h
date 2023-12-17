#pragma once
#include <SFML\Graphics.hpp>
#include "Framework/GameObject.h"
#include "Framework/Input.h"
#include "Framework/Collision.h"
#include <string>
#include <iostream>
#include "Client.h"
#include "Player.h"
#include "Menu.h"
using namespace sf;

class Player : public GameObject
{
public:
	Player();
	~Player();
	void init(Input* input, RenderWindow* window);
	void handleInput(float dt) override;
	void update(float dt, sf::Event* Event_);
	void clientInput(sf::Event* Event_);
	void collisionResponce(GameObject* collider);
	bool connected() { return connected_succesfully; };


	FloatRect getCollisionBox();
	void render();

	Vector2f playerStartPos;
	Vector2f playerPos;
private:
	RenderWindow* window;

	Texture txtr;
	float scale;
	Vector2f gravity;
	
};

