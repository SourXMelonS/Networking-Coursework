#pragma once
#include "Framework/GameObject.h"
#include "Framework/Input.h"
using namespace sf;

class MenuButton :
    public GameObject
{
public:
	MenuButton();
	MenuButton(Texture&, Input*, RenderWindow*, int);
	~MenuButton();

	void handleInput(float dt);
	void collisionResponse(GameObject*);
	void changePosition(Vector2f);

private:
	Texture textureHover;
};
};

