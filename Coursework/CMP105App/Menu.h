#pragma once
#include <SFML/Graphics.hpp>
#include "Framework/BaseLevel.h"
#include "Framework/Input.h"
#include "Framework/AudioManager.h"
#include "Framework/GameObject.h"
#include "Framework/GameState.h"
#include "Framework/Collision.h"
#include "MenuButton.h"
#include <string>
#include <iostream>

using namespace sf;

class Menu :
    public BaseLevel
{
public:
    Menu(RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud);
    ~Menu();
    void handleInput(float dt) override;
    void update(float dt) override;
    void render();

private:
    Texture backgroundTexture;
    Texture hostTexture;
    Texture joinTexture;
    Texture exitTexture;
    Text gameName;
    Font font;
    MenuButton hostButton;
    MenuButton joinButton;
    MenuButton exitButton;
    GameObject menuBackground;

    bool playedButton1 = false;
    bool playedButton2 = false;
    bool playedButton3 = false;
};

