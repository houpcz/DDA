#pragma once

#include "IGameState.h"
#include "IGraph.h"

class FlowGraph : public IGraph
{
public:
	virtual int GetValPlayer(int playerID, IGameState * previousState, IGameState * currentState)
	{
		if(playerID == 0)
			return 0;

		return currentState->GetPlayerScore(playerID, 0);
	}

	QString GetName() { return "Flow view"; }
	FlowGraph(void) {};
	~FlowGraph(void) {};
};

