#include "PlayerPOSM.h"
#include "IGame.h"
#include <QString>

using namespace std;

PlayerPOSM::PlayerPOSM(int _myID) : IPlayer(_myID)
{
	beta = 0.5;
}

PlayerPOSM::~PlayerPOSM(void)
{
}

void PlayerPOSM::StartGame(IGame * _game)
{
	IPlayer::StartGame(_game);
	for(int loop1 = 0; loop1 < W_LENGTH; loop1++)
		w[loop1] = 1.0;

	kdiff = 25;
}

void PlayerPOSM::UpdateDifficultyLevel()
{
	int o = 0;
	int h = game->GetCurrentState()->GetPlayerStatus(myID);
	if(h < 0)
		o = 1;
	else if(h > 0)
		o = -1;

	if(o != 0)
	{
        for(int loop1 = 0; loop1 < W_LENGTH; loop1++)
        {
            if(o == 1)
            {
                if(loop1 <= kdiff)
                {
                    w[loop1] = beta * w[loop1];
                }
            } else {
                if(loop1 >= kdiff)
                {
                    w[loop1] = beta * w[loop1];
                }
            }
        }
    }

    double sumUp = 0.0;
    double sumDown = 0.0;
    for(int loop1 = 0; loop1 < W_LENGTH; loop1++)
    {
        sumUp += w[loop1];
        A[loop1] = sumUp;
    }
    for(int loop1 = 0; loop1 < W_LENGTH; loop1++)
    {
        sumDown += w[W_LENGTH - loop1 - 1];
        B[W_LENGTH - loop1 - 1] = sumDown;
    }

    double bestValue = -1;
    int bestID = - 1;

    for(int loop1 = 0; loop1 < W_LENGTH; loop1++)
    {
        if(A[loop1] < B[loop1])
        {
            if(A[loop1] > bestValue)
            {
                bestValue = A[loop1];
                bestID = loop1;
            }
        } else {
            if(B[loop1] > bestValue)
            {
                bestValue = B[loop1];
                bestID = loop1;
            }
        }
    }

	kdiff = bestID;
	level = 51 + kdiff;
	//qDebug(QString::number(level).toAscii() + " -> " + QString::number(h).toAscii());
}

bool PlayerPOSM::Think()
{
	int choises;
	IGameState * currentState = game->GetCurrentState();
	IGameState ** nextState = currentState->GetNextStates(myID, &choises);

	UpdateDifficultyLevel();

	vector<valueIndex> scores;
	for(int loop1 = 0; loop1 < choises; loop1++)
	{
		scores.push_back(valueIndex(nextState[loop1]->GetPlayerRank(myID, myID), loop1));
	}
	sort(scores.begin(), scores.end(), comparator);
	
	int choise = (level / 100.0) * choises;
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
