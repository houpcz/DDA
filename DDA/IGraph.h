#pragma once

#include "IGameState.h"

class IGraph
{
public:
	virtual int GetValPlayer(int playerID, IGameState * gameState) = 0;
	IGraph(void);
	~IGraph(void);
};

