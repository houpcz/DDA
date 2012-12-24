#ifndef _MENSCHSTATE_H_
#define _MENSCHSTATE_H_

#include "igamestate.h"
class MenschState :
	public IGameState
{
public:
	static const int FIRST_HOME_TILE = 40;
	static const int MAX_PLAYER = 4;
	static const int MAX_FIGURE = 4;
	static const int MAX_CHOISES = MAX_FIGURE * 2;
private:
	int lastDice;
	int multipleDice;
	bool dicePlayerNow;
	int activePlayerID;
	int figure[MAX_PLAYER][MAX_FIGURE];
	int figureNextState[MAX_CHOISES];

	void NextChoises();
	bool IsPlayerWinner(int playerID) const;
public:
	MenschState(void);
	virtual ~MenschState(void);
	int GetFigure(int player, int number) { return figure[player][number]; }
	int GetFigureNextState(int number) { return figureNextState[number]; }
	int GetPlayerChoises(int whoAskID);
	int GetActivePlayerID() const;
	int GetPlayerScore(int playerID, int whoAskID);
	IGameState ** GetNextStates(int whoAskID, int *outNumberNextStates);
	int MakeTurn(int playerChoise);
	int GetLastDice() const { return lastDice; }
};

#endif;

