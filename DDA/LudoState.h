#ifndef _LUDOSTATE_H_
#define _LUDOSTATE_H_

#include "igamestate.h"

class LudoState :
	public IGameState
{
public:
	static const int FIRST_HOME_TILE = 40;
	static const int MAX_PLAYER = 4;
	static const int MAX_FIGURE = 4;
	static const int MAX_CHOISES = MAX_FIGURE * 2;
private:
	static const int safeTile[FIRST_HOME_TILE];
	int lastDice;
	int multipleDice;
	bool dicePlayerNow;
	int activePlayerID;
	int figure[MAX_PLAYER][MAX_FIGURE];
	int figureNextState[MAX_CHOISES];
	int playerScore[MAX_PLAYER];

	void CountPlayerScores();
	void NextChoises();
	bool IsPlayerWinner(int playerID) const;
	bool IsTileFreeCheckOpponents(int newPosition);
public:
	LudoState(void);
	virtual ~LudoState(void);
	ISpecificStat * GetGameSpecificStat();
	static bool IsTileSafe(int tileID);
	int GetFigure(int player, int number) { return figure[player][number]; }
	int GetFigureNextState(int number) { return figureNextState[number]; }
	int GetPlayerChoises(int whoAskID);
	int GetActivePlayerID() const;
	int GetPlayerScore(int playerID, int whoAskID);
	IGameState ** GetNextStates(int whoAskID, int *outNumberNextStates);
	int MakeTurn(int playerChoise);
	int GetLastDice() const { return lastDice; }
	bool IsGameOver();
};

#endif;

