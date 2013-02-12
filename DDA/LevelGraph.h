#pragma once

#include "IGameState.h"
#include "IGraph.h"

class LevelGraph : public IGraph
{
public:
	virtual int GetValPlayer(int playerID, IGameState * gameState)
	{
		return gameState->
	}

	LevelGraph(void) {};
	~LevelGraph(void) {};
};

