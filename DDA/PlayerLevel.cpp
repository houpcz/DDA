#include "PlayerLevel.h"
#include "IGame.h"

using namespace std;

PlayerLevel::PlayerLevel(int _myID) : IPlayer(_myID)
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
	if(level < 50) level = 50;
	if(level > 100) level = 100;

	//qDebug(QString::number(level).toAscii() + " -> " + QString::number(score).toAscii());
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
		scores.push_back(valueIndex(nextState[loop1]->GetPlayerRank(myID, myID), loop1));
	}
	sort(scores.begin(), scores.end(), comparator);
	
	int choise = (int) (level / 100.0) * choises;
	if(choise < 0)
		choise = 0;
	else if(choise >= choises)
		choise = choises - 1;

	for(int loop1 = 0; loop1 < choises; loop1++)
	{
		delete nextState[loop1];
	}
	delete [] nextState;

	myTurn = scores[choise].second;
	isReady = true;

	return true;
}
