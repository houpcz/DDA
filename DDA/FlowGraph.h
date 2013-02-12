#pragma once

#include "IGameState.h"
#include "IGraph.h"

class FlowGraph : public IGraph
{
public:
	virtual int GetValPlayer(int playerID, IGameState * gameState)
	{
		if(playerID == 0)
			return 0;

		return gameState->GetPlayerScore(playerID, 0);
	}

	FlowGraph(void) {};
	~FlowGraph(void) {};
};

