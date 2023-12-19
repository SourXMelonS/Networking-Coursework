#include "Coins.h"

Coins::Coins()
{
	picked = false;
	coinShape.setRadius(5);
	coinShape.setFillColor(sf::Color::Yellow);
	coinShape.setOutlineThickness(2);
	coinShape.setOutlineColor(sf::Color::White);
	setCollisionBox(0, 0, 10, 10);
}
Coins::~Coins()
{

}
void Coins::Render(sf::RenderWindow* window_)
{
	if (picked == false)
	{
		window_->draw(coinShape);
	}
}

void Coins::Update()
{
	coinShape.setPosition(getPosition());
}

void Coins::setPickedUp(bool picked_)
{
	picked = picked_;
}

bool Coins::getPickedUp()
{
	return picked;
}
