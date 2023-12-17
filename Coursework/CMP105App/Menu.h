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
#include <SFML/Network.hpp>
using namespace sf;

class Menu :
    public BaseLevel
{
public:
    Menu(RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud);
    ~Menu();
    void Init();
    void handleInput(float dt) override;
    void handleInput(float dt, sf::Event* event_);
    void update(float dt) override;
    void render() override;
    void reset();
    void enterName(sf::Event* event_);
    void enterIp(sf::Event* event_);
    void sendNameTCP();
    sf::IpAddress getIp();
    std::string getPlayerName();

private:
    bool connect_attempt = false;

    Texture backgroundTexture;
    Texture hostTexture;
    Texture joinTexture;
    Texture exitTexture;
    std::string ipEnterStr;
    Text gameName, ipDisplay,ipEnter, nameEnter, nameDisplay;
    Font font;
    MenuButton joinButton;
    MenuButton playButton;
    MenuButton exitButton;
    GameObject menuBackground, ipTextBox, nameTextBox;

    bool renderJoinGame = false;

    std::string players;
    std::string nameEnterStr;
    sf::IpAddress server_Ip;
    sf::TcpSocket* Tcp;
    std::string ipName;

    enum typeVar
    {
        sendName = 1,
    };

    bool playedButton1 = false;
    bool playedButton2 = false;
    bool playedButton3 = false;
};

