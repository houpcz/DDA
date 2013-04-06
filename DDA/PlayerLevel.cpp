#include "PlayerLevel.h"
#include "IGame.h"

using namespace std;

PlayerLevel::PlayerLevel(int _myID) : EnvironmentAI(_myID)
{
}

PlayerLevel::~PlayerLevel(void)
{
}


void PlayerLevel::StartGame(IGame * _game)
{
	IPlayer::StartGame(_game);

	level = 100;
}

void PlayerLevel::UpdateLevel()
{ 
	int score = game->GetCurrentState()->GetPlayerStatus(myID);
	if(score < 0) level += (int) log((double) -score);
	if(score > 0) level -= (int) log((double) score);
	if(level < 0) level = 0;
	if(level > 100) level = 100;
}


bool PlayerLevel::Think()
{
	int choises;
	IGameState * currentState = game->GetCurrentState();
	IGameState ** nextState = currentState->GetNextStates(myID, &choises);

	UpdateLevel();

	vector<valueIndex> scores;
	for(int loop1 = 0; loop1 < choises; loop1++)
	{
		if(nextState[loop1] == NULL)
			continue;

		scores.push_back(valueIndex(nextState[loop1]->GetPlayerRank(myID, myID), loop1));
	}
	sort(scores.begin(), scores.end(), comparator);
	int realChoises = scores.size();
	
	int choise = (int) (level / 100.0) * realChoises;
	if(choise < 0)
		choise = 0;
	else if(choise >= realChoises)
		choise = realChoises - 1;

	for(int loop1 = 0; loop1 < choises; loop1++)
	{
		if(nextState[loop1] == NULL)
			continue;

		delete nextState[loop1];
	}
	delete [] nextState;

	myTurn = scores[choise].second;
	isReady = true;

	return true;
}
