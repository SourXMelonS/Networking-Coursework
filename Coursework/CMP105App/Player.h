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
	//void init(Input* input, RenderWindow* window);
	void Init();
	void handleInput(Input* input,float dt);
	void update(float dt);
	void collisionResponce(GameObject* collider);
	

	RectangleShape playerBody;
	FloatRect getCollisionBox();
	void render(sf::RenderWindow* window_);

	Vector2f playerStartPos;
	//Vector2f playerDirection;
	Vector2f playerPos;
	//Vector2f playerNextPos;
	Vector2f nextPos = Vector2f(0, 0);		

	float tempT;				
	int score;
	int id;
private:
	//RenderWindow* window;
	Texture txtr;
	
};

