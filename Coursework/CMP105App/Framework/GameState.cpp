#include "GameState.h"

// Set the current state
void GameState::setCurrentState(State s)
{
	currentState = s;
}

void GameState::setServerState(IsServer s)
{
	currentServer = s;
}

// Returns the current state.
State GameState::getCurrentState()
{
	return currentState;
}

IsServer GameState::getCurrentServer()
{
	return currentServer;
}
