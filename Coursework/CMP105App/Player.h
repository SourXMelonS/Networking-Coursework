#pragma once
#include <SFML\Graphics.hpp>
#include "Framework/GameObject.h"
#include "Framework/Input.h"
#include "Framework/Collision.h"
#include <string>
#include <sstream>     
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace sf;

class Player : public GameObject
{
public:
	Player();
	~Player();
	void init(Input* input, RenderWindow* window);
	void handleInput(float dt) override;
	void update(float dt);
	void collisionResponce(GameObject* collider);

	RectangleShape playerBody;
	FloatRect getCollisionBox();
	void render();

	Vector2f playerStartPos;
	Vector2f playerPos;
	Vector2f playerNextPos;

	int m_id;
private:
	RenderWindow* window;

	Texture txtr;
	float scale;
	Vector2f gravity;
	
};

