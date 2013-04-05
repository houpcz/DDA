#ifndef _LUDOSTATE_H_
#define _LUDOSTATE_H_

#include "igamestate.h"

class LudoState :
	public IGameState
{
public:
	static const int FIRST_HOME_TILE = 40;
	static const int MAX_CUBE = 6;
	static const int MAX_PLAYER = 4;
	static const int MAX_FIGURE = 4;
	static const int MAX_CHOISES = MAX_FIGURE * 2;
	static const int MAX_LAST_DICE = 4;
private:
	bool twoPlayers;
	static const int safeTile[FIRST_HOME_TILE];
	int lastDice;
	int multipleDice;
	bool dicePlayerNow;
	int activePlayerID;
	int figure[MAX_PLAYER][MAX_FIGURE];
	int figureNextState[MAX_CHOISES];
	int playerRank[MAX_PLAYER];
	int credibility[MAX_PLAYER][MAX_CUBE];
	int credibilityRecent[MAX_PLAYER][MAX_LAST_DICE];
	bool isRankUpToDate;

	void CountPlayerRanks();
	void NextChoises();
	bool IsPlayerWinner(int playerID) const;
	bool IsTileFreeCheckOpponents(int newPosition);
public:
	LudoState(Game * _game, bool _twoPlayers);
	virtual ~LudoState(void);
	LudoState(const LudoState & origin);
	LudoState& operator=(const LudoState &origin);
	void CopyToMe(const LudoState & origin);
	ISpecificStat * GetGameSpecificStat();
	static bool IsTileSafe(int tileID);
	LudoState * Clone() { LudoState * state = new LudoState(*this);  return state; };
	int GetFigure(int player, int number) { return figure[player][number]; }
	int GetFigureNextState(int number) { return figureNextState[number]; }
	int GetPlayerChoises(int whoAskID);
	int GetActivePlayerID() const;
	int GetPlayerRank(int playerID, int whoAskID);
	int GetPlayerStatus(int playerID);
	float FreedomLimit() { return 4.0f; };
	IGameState ** GetNextStates(int whoAskID, int *outNumberNextStates);
	IGameState * GetRandomNextState(int whoAskID, int * outStateID);
	IGameState * SimulateToTheEnd(int whoAskID);
	float GetNextStateProb(int whoAskID, int actionID) { return 1.0f; };
	int MakeTurn(int playerChoise);
	int GetLastDice() const { return lastDice; }
	float GetCredibility();
	bool IsGameOver();
};

#endif;

