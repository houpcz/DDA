#include "MenschState.h"
#include "MenschArgere.h"

MenschState::MenschState(void)
{
	for(int loop1 = 0; loop1 < MAX_PLAYER; loop1++)
	{
		figure[loop1][0] = 0;
		for(int loop2 = 0; loop2 < MAX_FIGURE; loop2++)
		{
			figure[loop1][loop2] = loop2;
		}
	}

	dicePlayerNow = true;
	activePlayerID = 0;
	lastDice = 0;
	multipleDice = 2;
}


MenschState::~MenschState(void)
{
}

IGameState ** MenschState::GetNextStates(int whoAskID, int *outNumberNextStates)
{
	int numberNextStates = GetPlayerChoises(whoAskID);
	IGameState ** nextState = new IGameState*[numberNextStates];
	MenschState * menschState;
	for(int loop1 = 0; loop1 < numberNextStates; loop1++)
	{
		menschState = new MenschState(*this);
		menschState->MakeTurn(loop1);
		nextState[loop1] = menschState;
	}

	*outNumberNextStates = numberNextStates;

	return nextState;
}

int MenschState::GetPlayerChoises(int whoAskID)
{
	if(dicePlayerNow)
	{
		return 6;
	}

	int choises = 0;
	
	for(int loop1 = 0; loop1 < MAX_CHOISES; loop1++)
	{
		if(figureNextState[loop1] >= 0)
			choises++;
	}
	return (choises > 0) ? choises : 1;
}

void MenschState::NextChoises()
{
	const int START_TILE = 0;
	bool isSomeFigurePreparedToGoToStart = false;
	for(int loop1 = 0; loop1 < MAX_FIGURE; loop1++)
	{
		figureNextState[loop1] = -2;
		figureNextState[loop1 + MAX_FIGURE] = -2;

		if(figure[activePlayerID][loop1] < 0 && lastDice == 6 && !isSomeFigurePreparedToGoToStart)
		{
			bool isFreeStart = true;
			for(int loop2 = 0; loop2 < MAX_FIGURE; loop2++)
			{
				if(figure[activePlayerID][loop2] == START_TILE)
				{
					isFreeStart = false;
					break;
				}
			}

			if(isFreeStart)
			{
				figureNextState[loop1] = START_TILE;
				isSomeFigurePreparedToGoToStart = true;
			}
		} else {
			if(figure[activePlayerID][loop1] + lastDice < 44 && figure[activePlayerID][loop1] >= 0) 
			{
				bool isFreeTile = true;
				for(int loop2 = 0; loop2 < MAX_FIGURE; loop2++)
				{
					if(figure[activePlayerID][loop2] == figure[activePlayerID][loop1] + lastDice)
					{
						isFreeTile = false;
						break;
					}
				}

				if(isFreeTile)
					figureNextState[loop1] = figure[activePlayerID][loop1] + lastDice;
			}

			if(figure[activePlayerID][loop1] < FIRST_HOME_TILE && figure[activePlayerID][loop1] - lastDice >= 0 && figure[activePlayerID][loop1] >= 0) 
			{
				bool isFreeTile = true;
				for(int loop2 = 0; loop2 < MAX_FIGURE; loop2++)
				{
					if(figure[activePlayerID][loop2] == figure[activePlayerID][loop1] - lastDice)
					{
						isFreeTile = false;
						break;
					}
				}

				if(isFreeTile)
					figureNextState[loop1 + MAX_FIGURE] = figure[activePlayerID][loop1] - lastDice;
			}
		}
	}
}

int MenschState::MakeTurn(int playerChoise)
{
	int result = -1;

	if(dicePlayerNow)
	{
		lastDice = playerChoise + 1;
		NextChoises();
	} else {
		int choise = 0;
		for(int loop1 = 0; loop1 < MAX_CHOISES; loop1++)
		{
			if(figureNextState[loop1] >= 0)
			{
				if(choise == playerChoise)
				{
					int figureId = loop1 % MAX_FIGURE;
					figure[activePlayerID][figureId] = figureNextState[loop1];
					int tileID1 = (figure[activePlayerID][figureId] + MenschArgere::firstTile[activePlayerID]) % MenschArgere::PLAYER_1_START;
					for(int loop2 = 0; loop2 < MAX_PLAYER; loop2++)
					{
						if(loop2 == activePlayerID)
							continue;
						for(int loop3 = 0; loop3 < MAX_PLAYER; loop3++)
						{
							int tileID2 = (figure[loop2][loop3] + MenschArgere::firstTile[loop2]) % MenschArgere::PLAYER_1_START;
							if(tileID1 == tileID2 &&
								figure[loop2][loop3] >= 0 &&
								figure[loop2][loop3] < MenschArgere::PLAYER_1_START)
							{
								figure[loop2][loop3] = -1;
								break;
							}
						}
					}

					break;
				}
				choise++;
			}
		}

		
		if(IsPlayerWinner(activePlayerID))
			result = activePlayerID;

		if(lastDice != 6)
		{
			bool hasFigureInField = false;
			for(int loop1 = 0; loop1 < MAX_FIGURE; loop1++)
			{
				if(figureNextState[loop1] >= 0)
				{
					hasFigureInField = true;
					break;
				}
			}

			if(multipleDice > 0 && !hasFigureInField)
			{
				multipleDice--;
			} else {
				multipleDice = 2;
				activePlayerID = (activePlayerID + 1) % 4;
			}
		}
	}
	dicePlayerNow = !dicePlayerNow;

	return result;
}

int MenschState::GetActivePlayerID() const
{
	if(dicePlayerNow)
		return 0;

	return activePlayerID + 1;
}

bool MenschState::IsPlayerWinner(int playerID) const
{
	bool isWinner = true;
	for(int loop1 = 0; loop1 < MAX_FIGURE; loop1++)
	{
		if(figure[playerID][loop1] < 40)
		{
			isWinner = false;
			break;
		}
	}
	return isWinner;
}

int MenschState::GetPlayerScore(int playerID, int whoAskID)
{
	//if(IsPlayerWinner(playerID))
	//	return IGameState::WINNER_SCORE;

	int result = 0;
	for(int loop1 = 0; loop1 < MAX_FIGURE; loop1++)
	{
		if(figure[playerID - 1][loop1] >= FIRST_HOME_TILE)
		{
			result += 50;
		} else if(figure[playerID - 1][loop1] >= 0)
		{
			result += 10 + figure[playerID - 1][loop1];
		}
	}
	return result;
}
