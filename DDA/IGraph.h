#pragma once

#include "IGameState.h"

class IGraph
{
public:
	virtual int GetValPlayer(int playerID, IGameState * previousState, IGameState * currentState) = 0;
	virtual QString GetName() = 0;
	IGraph(void);
	~IGraph(void);
};

