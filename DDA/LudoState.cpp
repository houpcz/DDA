
#include "MatrixFactory.h"
#include "Ludo.h"


const int LudoState::safeTile[] = {0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 
								   0, 0, 0, 1, 0, 0, 0, 1, 0, 0,
								   0, 0, 0, 1, 0, 0, 0, 1, 0, 0,
								   0, 0, 0, 1, 0, 0, 0, 1, 0, 0};

LudoState::LudoState(Game * _game) : IGameState(_game)
{
	playerCount = 5;
	for(int loop1 = 0; loop1 < MAX_PLAYER; loop1++)
	{
		playerRank[loop1] = 0;
		figure[loop1][0] = 0;
		for(int loop2 = 0; loop2 < MAX_FIGURE; loop2++)
		{
			figure[loop1][loop2] = loop2;
		}

		for(int loop2 = 0; loop2 < MAX_CUBE; loop2++)
		{
			credibility[loop1][loop2] = 0;
		}

		for(int loop2 = 0; loop2 < MAX_LAST_DICE; loop2++)
		{
			credibilityRecent[loop1][loop2] = -1;
		}
	}

	dicePlayerNow = true;
	isRankUpToDate = false;
	activePlayerID = 0;
	lastDice = 0;
	multipleDice = 2;
	UpdateGameStat();
}

LudoState::LudoState(const LudoState & origin)
{
	IGameState::Init(origin.game, origin.gameStat);
	CopyToMe(origin);
}

LudoState& LudoState::operator=(const LudoState &origin)
{
	if(&origin != this)
	{
		CopyToMe(origin);
	}

	return *this;
}

void LudoState::CopyToMe(const LudoState & origin)
{
	lastDice = origin.lastDice;
	multipleDice = origin.multipleDice;
	dicePlayerNow = origin.dicePlayerNow;
	activePlayerID = origin.activePlayerID;
	for(int loop1 = 0; loop1 < MAX_PLAYER; loop1++)
	{
		playerRank[loop1] = origin.playerRank[loop1];
		for(int loop2 = 0; loop2 < MAX_FIGURE; loop2++)
		{
			figure[loop1][loop2] = origin.figure[loop1][loop2];
		}

		for(int loop2 = 0; loop2 < MAX_CUBE; loop2++)
		{
			credibility[loop1][loop2] = origin.credibility[loop1][loop2];
		}

		for(int loop2 = 0; loop2 < MAX_LAST_DICE; loop2++)
		{
			credibilityRecent[loop1][loop2] = origin.credibilityRecent[loop1][loop2];
		}
	}
	for(int loop1 = 0; loop1 < MAX_CHOISES; loop1++)
	{
		figureNextState[loop1] = origin.figureNextState[loop1];
	}

	isRankUpToDate = origin.isRankUpToDate;
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

IGameState * LudoState::GetRandomNextState(int whoAskID, int * outStateID)
{
	int numberNextStates = GetPlayerChoises(whoAskID);
	int turn = rand() % numberNextStates;
	LudoState * ludoState = new LudoState(*this);
	ludoState->MakeTurn(turn);

	*outStateID = turn;
	return ludoState;
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
	if(newPosition >= FIRST_HOME_TILE || newPosition < 0)
		return true;

	int tileID1 = (newPosition + Ludo::firstTile[activePlayerID]) % Ludo::PLAYER_1_START;
	for(int loop3 = 0; loop3 < MAX_PLAYER; loop3++)
	{
		if(loop3 == activePlayerID)
			continue;

		for(int loop2 = 0; loop2 < MAX_FIGURE; loop2++)
		{
			if(figure[loop3][loop2] < 0 || figure[loop3][loop2] >= FIRST_HOME_TILE)
				continue;

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

			/*
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
			}*/
		}
	}
}

int LudoState::MakeTurn(int playerChoise)
{
	int result = -1;

	if(dicePlayerNow)
	{
		credibility[activePlayerID][playerChoise]++;
		for(int loop1 = MAX_LAST_DICE - 1; loop1 > 0; loop1--)
		{
			credibilityRecent[activePlayerID][loop1] = credibilityRecent[activePlayerID][loop1 - 1];
		}
		credibilityRecent[activePlayerID][0] = playerChoise;

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

					// you cant sent you opponent to home
					if(figureNextState[loop1] < 0 || figureNextState[loop1] >= FIRST_HOME_TILE)
						break; 

					int tileID1 = (figure[activePlayerID][figureId] + Ludo::firstTile[activePlayerID]) % Ludo::PLAYER_1_START;
					for(int loop2 = 0; loop2 < MAX_PLAYER; loop2++)
					{
						if(loop2 == activePlayerID)
							continue;
						for(int loop3 = 0; loop3 < MAX_FIGURE; loop3++)
						{
							if(figure[loop2][loop3] < 0 || figure[loop2][loop3] >= FIRST_HOME_TILE)
								continue;

							int tileID2 = (figure[loop2][loop3] + Ludo::firstTile[loop2]) % Ludo::PLAYER_1_START;
							if(tileID1 == tileID2)
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
		} else {
			activePlayerID = (activePlayerID + 1) % 4;
		}
	}
	dicePlayerNow = !dicePlayerNow;

	isRankUpToDate = false;

	UpdateGameStat();

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

int LudoState::GetPlayerRank(int playerID, int whoAskID)
{
	if(!isRankUpToDate)
		CountPlayerRanks();

	return playerRank[playerID - 1];
}

int LudoState::GetPlayerStatus(int playerID)
{
	return GetPlayerRank(playerID, 0);
}

void LudoState::CountPlayerRanks()
{
	if(dicePlayerNow)
	{
		int tempRank[MAX_PLAYER];
		char board[FIRST_HOME_TILE];
		for(int loop1 = 0; loop1 < FIRST_HOME_TILE; loop1++)
			board[loop1] = -1;

		int bestRank = 0;
		int secondBest = 0;
		for(int loop2 = 0; loop2 < MAX_PLAYER; loop2++)
		{
			int result = 0;
			for(int loop1 = 0; loop1 < MAX_FIGURE; loop1++)
			{
				int boardID = (figure[loop2][loop1] + Ludo::firstTile[loop2]) % FIRST_HOME_TILE;
				board[boardID] = loop2;
				if(safeTile[boardID] == 1)
					result += 2;

				if(figure[loop2][loop1] >= FIRST_HOME_TILE)
				{
					//result += 4;
				} else if(figure[loop2][loop1] >= 0)
				{
					result -= (int) (((FIRST_HOME_TILE - figure[loop2][loop1]) / 3.5f) + 0.5f) + 4;
				} else {
					result -= (int) ((FIRST_HOME_TILE / 3.5f) + 0.5f + 3.0f + 4.0f/3.0f);
				}
			}
			tempRank[loop2] = result + 100;
		}

		for(int loop1 = 0; loop1 < FIRST_HOME_TILE; loop1++)
		{
			if(board[loop1] > 0)
			{
				for(int loop2 = 1; loop2 < 7; loop2++)
				{
					int opponentPos = (loop1 + loop2) % FIRST_HOME_TILE;
					int opponentID = board[opponentPos];
					if(safeTile[opponentPos] == 0 && board[opponentPos] > 0 && opponentID != board[loop1])
					{
						tempRank[opponentID] -= (int) (((((FIRST_HOME_TILE + opponentPos - Ludo::firstTile[opponentID]) % FIRST_HOME_TILE) / 3.5f + 3.0f + 4.0f/3.0f) / 6.0f) + 0.5f);
					}
				}
			}
		}


		for(int loop2 = 0; loop2 < MAX_PLAYER; loop2++)
		{
			if(tempRank[loop2] > secondBest)
			{
				if(tempRank[loop2] > bestRank)
				{
					secondBest = bestRank;
					bestRank = tempRank[loop2];
				} else
				{
					secondBest = tempRank[loop2];
				}
			}
		}

		for(int loop1 = 0; loop1 < MAX_PLAYER; loop1++)
		{
			if(tempRank[loop1] == bestRank)
				playerRank[loop1] = tempRank[loop1] - secondBest;
			else
				playerRank[loop1] = tempRank[loop1] - bestRank;
		}
	} else {
			int choises;
			IGameState ** nextState = GetNextStates(GetActivePlayerID(), &choises);
			int maxMyRank = INT_MIN;
			int maxMyRankID = -1;
			for(int loop1 = 0; loop1 < choises; loop1++)
			{
				int temp = nextState[loop1]->GetPlayerRank(GetActivePlayerID(), GetActivePlayerID());
				if(temp > maxMyRank)
				{
					maxMyRank = temp;
					maxMyRankID = loop1;
				}
			}
			for(int loop1 = 0; loop1 < MAX_PLAYER; loop1++)
			{
				playerRank[loop1] = nextState[maxMyRankID]->GetPlayerRank(loop1 + 1, GetActivePlayerID());
			}
			for(int loop1 = 0; loop1 < choises; loop1++)
			{
				delete nextState[loop1];
			}
			delete [] nextState;
	}

	isRankUpToDate = true;
}

IGameState * LudoState::SimulateToTheEnd(int whoAskID)
{
	LudoState * state = new LudoState(*this);

	while(!state->IsGameOver())
	{
		state->MakeTurn(rand() % GetPlayerChoises(whoAskID));
	}
	
	return state;
}

ISpecificStat * LudoState::GetGameSpecificStat()
{
	return NULL;
}

float LudoState::GetCredibility()
{
	float credSum = 0.0f;
	for(int loop1 = 0; loop1 < MAX_PLAYER; loop1++)
	{
		credSum += MatrixFactory::Inst()->Credibility(credibility[loop1], MAX_CUBE);

		float credBonus = 0.0;
		if(credibilityRecent[loop1][0] == credibilityRecent[loop1][1])
		{
			credBonus = 1.0;
			for(int loop2 = 2; loop2 < MAX_LAST_DICE; loop2++)
			{
				if(credibilityRecent[loop1][loop2] > 0)
				{
					if(credibilityRecent[loop1][loop2] == credibilityRecent[loop1][loop2 - 1])
					{
						credBonus *= 15.0f;
					} else
						break;
				}
			}
		}
		credSum += credBonus;
	}

	return credSum;
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