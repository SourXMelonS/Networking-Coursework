#pragma once
#include "Framework\GameObject.h"

class Coins : public GameObject
{
public:
	Coins();
	~Coins();
	void Render(sf::RenderWindow* window_);
	void Update();
	void setPickedUp(bool picked_);
	bool getPickedUp();
protected:
	sf::CircleShape coinShape;
	bool picked;
};

