// Game State Class
// Simple game state controller. enum lists possible game states (can be modified), tracks and updates the current state
// @author Paul Robertson

#pragma once

// Possible states. This enum can be modified to add additional states as required.
enum class State { MENU,JOIN,HOST, LEVEL, PAUSE,VICTORY, CREDITS };
enum class IsServer {HOST, CLIENT};

class GameState
{
public:
	// Set the current state
	void setCurrentState(State s);
	void setServerState(IsServer s);
	// Returns the current state.
	State getCurrentState();
	IsServer getCurrentServer();

	

protected:
	State currentState;
	IsServer currentServer;
	
};