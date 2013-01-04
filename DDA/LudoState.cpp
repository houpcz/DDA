
#include "Ludo.h"


const int LudoState::safeTile[] = {0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 
								   0, 0, 0, 1, 0, 0, 0, 1, 0, 0,
								   0, 0, 0, 1, 0, 0, 0, 1, 0, 0,
								   0, 0, 0, 1, 0, 0, 0, 1, 0, 0};

LudoState::LudoState(void)
{
	for(int loop1 = 0; loop1 < MAX_PLAYER; loop1++)
	{
		playerScore[loop1] = 0;
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


LudoState::~LudoState(void)
{
}

IGameState ** LudoState::GetNextStates(int whoAskID, int *outNumberNextStates)
{
	int numberNextStates = GetPlayerChoises(whoAskID);
	IGameState ** nextState = new IGameState*[numberNextStates];
	LudoState * ludoState; 
	for(int loop1 = 0; loop1 < numberNextStates; loop1++)
	{
		ludoState = new LudoState(*this);
		ludoState->MakeTurn(loop1);
		nextState[loop1] = ludoState; 
	}

	*outNumberNextStates = numberNextStates;

	return nextState;
}

int LudoState::GetPlayerChoises(int whoAskID)
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

bool LudoState::IsTileFreeCheckOpponents(int newPosition)
{
	int tileID1 = (newPosition + Ludo::firstTile[activePlayerID]) % Ludo::PLAYER_1_START;
	for(int loop3 = 0; loop3 < MAX_PLAYER; loop3++)
	{
		if(loop3 == activePlayerID)
			continue;

		for(int loop2 = 0; loop2 < MAX_FIGURE; loop2++)
		{
			int tileID2 = (figure[loop3][loop2] + Ludo::firstTile[loop3]) % Ludo::PLAYER_1_START;
			if(tileID2 == tileID1)
			{
				return false;
			}
		}
	}
	return true;
}

void LudoState::NextChoises()
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
			int oldPosition = figure[activePlayerID][loop1];
			int newPosition = figure[activePlayerID][loop1] + lastDice;
			if(newPosition < 44 && oldPosition >= 0) 
			{
				bool isFreeTile = true;
				for(int loop2 = 0; loop2 < MAX_FIGURE; loop2++)
				{
					// you cant go to position of your others figures
					if(figure[activePlayerID][loop2] == newPosition)
					{
						isFreeTile = false;
						break;
					}
				}
				if(isFreeTile && LudoState::IsTileSafe(newPosition))
				{
					isFreeTile = IsTileFreeCheckOpponents(newPosition);
				}

				if(isFreeTile)
					figureNextState[loop1] = figure[activePlayerID][loop1] + lastDice;
			}

			newPosition = figure[activePlayerID][loop1] - lastDice;
			if(figure[activePlayerID][loop1] < FIRST_HOME_TILE && newPosition >= 0 && oldPosition >= 0) 
			{
				bool isFreeTile = true;
				for(int loop2 = 0; loop2 < MAX_FIGURE; loop2++)
				{
					if(figure[activePlayerID][loop2] == newPosition)
					{
						isFreeTile = false;
						break;
					}
				}

				if(isFreeTile && LudoState::IsTileSafe(newPosition))
				{
					isFreeTile = IsTileFreeCheckOpponents(newPosition);
				}

				if(isFreeTile)
					figureNextState[loop1 + MAX_FIGURE] = figure[activePlayerID][loop1] - lastDice;
			}
		}
	}
}

int LudoState::MakeTurn(int playerChoise)
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
					int tileID1 = (figure[activePlayerID][figureId] + Ludo::firstTile[activePlayerID]) % Ludo::PLAYER_1_START;
					for(int loop2 = 0; loop2 < MAX_PLAYER; loop2++)
					{
						if(loop2 == activePlayerID)
							continue;
						for(int loop3 = 0; loop3 < MAX_PLAYER; loop3++)
						{
							int tileID2 = (figure[loop2][loop3] + Ludo::firstTile[loop2]) % Ludo::PLAYER_1_START;
							if(tileID1 == tileID2 &&
								figure[loop2][loop3] >= 0 &&
								figure[loop2][loop3] < Ludo::PLAYER_1_START)
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


	CountPlayerScores();

	return result;
}

int LudoState::GetActivePlayerID() const
{
	if(dicePlayerNow)
		return 0;

	return activePlayerID + 1;
}

bool LudoState::IsPlayerWinner(int playerID) const
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

bool LudoState::IsTileSafe(int tileID)
{
	if(tileID < 0 || tileID >= FIRST_HOME_TILE)
		return true;
	else return safeTile[tileID];
}

int LudoState::GetPlayerScore(int playerID, int whoAskID)
{
	return playerScore[playerID - 1];
}
void LudoState::CountPlayerScores()
{
	int tempScore[MAX_PLAYER];

	int bestScore = 0;
	int secondBest = 0;
	for(int loop2 = 0; loop2 < MAX_PLAYER; loop2++)
	{
		int result = 0;
		for(int loop1 = 0; loop1 < MAX_FIGURE; loop1++)
		{
			if(figure[loop2][loop1] >= FIRST_HOME_TILE)
			{
				result += 50;
			} else if(figure[loop2][loop1] >= 0)
			{
				result += 10 + figure[loop2][loop1];
			}
		}
		tempScore[loop2] = result;

		if(result > secondBest)
		{
			if(result > bestScore)
			{
				secondBest = bestScore;
				bestScore = result;
			} else
			{
				secondBest = result;
			}
		}
	}

	for(int loop1 = 0; loop1 < MAX_PLAYER; loop1++)
	{
		if(tempScore[loop1] == bestScore)
			playerScore[loop1] = tempScore[loop1] - secondBest;
		else
			playerScore[loop1] = tempScore[loop1] - bestScore;
	}
}

bool LudoState::IsGameOver()
{
	for(int loop1 = 0; loop1 < MAX_PLAYER; loop1++)
	{
		if(IsPlayerWinner(loop1))
			return true;
	}
	return false;
}