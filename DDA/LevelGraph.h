#pragma once

#include "IGameState.h"
#include "IGraph.h"

class LevelGraph : public IGraph
{
public:
	virtual int GetValPlayer(int playerID, IGameState * previousState, IGameState * currentState)
	{
		if(previousState == NULL)
			return currentState->Statistics()->PlayerLevel(playerID);
		else
			return currentState->Statistics()->PlayerLevel(playerID) - previousState->Statistics()->PlayerLevel(playerID);
	}

	QString GetName() { return "Level view"; }
	LevelGraph(void) {};
	~LevelGraph(void) {};
};

