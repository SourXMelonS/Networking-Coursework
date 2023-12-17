#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include "client.h"
#include "server.h"
server server_;
void ServerInit()
{
	server_.Init();
}
int main()
{
	std::cout << "Server is starting :3 \n";

	ServerInit();

    server_.Update();
}