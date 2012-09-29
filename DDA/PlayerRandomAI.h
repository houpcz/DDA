#pragma once
#include "iplayer.h"
class PlayerRandomAI :
	public IPlayer
{
public:
	PlayerRandomAI(void);
	~PlayerRandomAI(void);
	virtual bool Think();
	void HumanTurn(int turn) {};
};

