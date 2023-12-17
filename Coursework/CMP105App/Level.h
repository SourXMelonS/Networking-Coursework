#pragma once

#include <SFML/Graphics.hpp>
#include "Framework/BaseLevel.h"
#include "Player.h"

class Level : BaseLevel{
public:
	Level(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud);
	~Level();

	void handleInput(float dt) override;
	void update(float dt);
	void render();
	Player getPlayer(int player_);

private:
	// Default functions for rendering to the screen.
	Player player[2];

	int speed;
	Font font;
	Text controls, speed;
	View gameView;
	// Default variables for level class.
	
};