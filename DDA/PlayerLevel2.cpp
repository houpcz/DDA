#include "PlayerLevel2.h"
#include "IGame.h"

using namespace std;

PlayerLevel2::PlayerLevel2(int _myID) : IPlayer(_myID)
{
}

PlayerLevel2::~PlayerLevel2(void)
{
}


void PlayerLevel2::StartGame(IGame * _game)
{
	IPlayer::StartGame(_game);

	level = 100;
}

void PlayerLevel2::UpdateLevel()
{ 
	int score = game->GetCurrentState()->GetPlayerStatus(myID);
	if(score < 0) level += (int) log((double) -score);
	if(score > 0) level -= (int) log((double) score);
	if(level < 0) level = 0;
	if(level > 100) level = 100;
}


bool PlayerLevel2::Think()
{
	int choises;
	IGameState * currentState = game->GetCurrentState();
	int initRank = currentState->GetPlayerRank(myID, myID);
	IGameState ** nextState = currentState->GetNextStates(myID, &choises);

	UpdateLevel();

	int bestID = -1;
	int bestRank = initRank;
	vector<valueIndex> scores;
	for(int loop1 = 0; loop1 < choises; loop1++)
	{
		int nextRank = nextState[loop1]->GetPlayerRank(myID, myID);
		if(nextRank >= bestRank || loop1 == 0)
		{
			bestRank = nextRank;
			bestID = loop1;
		}

		if(nextRank >= initRank)
			scores.push_back(valueIndex(nextRank, loop1));
	}

	sort(scores.begin(), scores.end(), comparator);
	
	choises = scores.size();
	int choise = (int) (level / 100.0) * choises;
	if(choise < 0)
		choise = 0;
	else if(choise >= choises)
		choise = choises - 1;

	if(scores.size() == 0)
		myTurn = bestID;
	else
		myTurn = scores[choise].second;

	for(int loop1 = 0; loop1 < choises; loop1++)
	{
		delete nextState[loop1];
	}
	delete [] nextState;

	isReady = true;

	return true;
}
