#pragma once
#include "Framework/GameObject.h"
#include "Framework/Input.h"
#include "Framework/Collision.h"

using namespace sf;

class Player : public GameObject
{
public:
	Player();
	~Player();
	void init(Input* input, RenderWindow* window);
	void handleInput(float dt) override;
	void update(float dt) override;
	void collisionResponce(GameObject* collider);

	FloatRect getCollisionBox();
	void render();
private:
	RenderWindow* window;

	Texture txtr;
	float scale;
	Vector2f gravity;
};

