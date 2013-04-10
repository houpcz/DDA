#include "MonteCarloTreeSearch.h"
#include <vector>
#include "IPlayer.h"
#include "Utility.h"

using namespace std;

MonteCarloNode::MonteCarloNode(IGameState * _myState, MonteCarloNode * _parent, int _turnID)
{
	parent = _parent;
	myState = _myState;
	visits = 0;
	value = 0.0;
	childrenNumber = 0;
	children = NULL;
	turnID = _turnID;
}

MonteCarloNode::~MonteCarloNode()
{
	if(children != NULL)
	{
		for(int loop1 = 0; loop1 < childrenNumber; loop1++)
		{
			delete children[loop1];
		}
		delete [] children;
	}

	if(parent != NULL)
			delete myState;
}

void MonteCarloNode::Expansion(int whoAskID)
{
	if(myState->IsGameOver())
		return;

	int choises;
	IGameState ** nextStates = myState->GetNextStates(whoAskID, &choises);
	childrenNumber = 0;
	for(int loop1 = 0; loop1 < choises; loop1++)
	{
		if(nextStates[loop1] != NULL)
			childrenNumber++;
	}

	children = new MonteCarloNode*[childrenNumber];

	int childNumber = 0;
	for(int loop1 = 0; loop1 < choises; loop1++)
	{
		if(nextStates[loop1] != NULL)
			children[childNumber++] = new MonteCarloNode(nextStates[loop1], this, loop1);
	}
	delete [] nextStates;
}

int MonteCarloNode::GetBestChildID()
{
	double bestValue = children[0]->value;
	int bestID = children[0]->turnID;
	for(int loop1 = 1; loop1 < childrenNumber; loop1++)
	{
		if(children[loop1]->value > bestValue)
		{
			bestValue = children[loop1]->value;
			bestID = children[loop1]->turnID;
		}
	}
	return bestID;
	/*
	vector<valueDIndex> scores;
	for(int loop1 = 0; loop1 < childrenNumber; loop1++)
	{
		scores.push_back(valueDIndex(children[loop1]->value, children[loop1]->turnID));
	}
	sort(scores.begin(), scores.end(), comparatorD);
	return scores[Utility::Inst()->GetTurnIDByLevel(scores.size(), level)].second;
	*/
}

void MonteCarloNode::Backpropagation(float * values)
{
	visits++;

	float newReward = 0.0f;
	for(int loop1 = 0; loop1 < COEF_COUNT; loop1++)
	{
		if(values[loop1] < minValue[loop1])
			minValue[loop1] = values[loop1];
		if(values[loop1] > maxValue[loop1])
			maxValue[loop1] = values[loop1];

		newReward += maxValue[loop1] - minValue[loop1];
	}

	value += 1.0f / visits * (newReward - value);
	

	if(parent != NULL)
	{
		parent->Backpropagation(values);
	}
}

void MonteCarloNode::Backpropagation(double reward)
{
	visits++;
	value += 1.0f / visits * (reward - value);
	
	if(parent != NULL)
	{
		parent->Backpropagation(reward);
	}
}

MonteCarloNode * MonteCarloNode::GetRandomChild()
{
	return children[rand() % childrenNumber];
}
MonteCarloNode * MonteCarloNode::GetBestChild(int whoAskID)
{
	static double c = 1.0;
	int bestID = 0;

	vector<int> notVisitedNodes;
	float sumProb = 0.0;
	for(int loop1 = 0; loop1 < childrenNumber; loop1++)
	{
		if(children[loop1]->visits == 0)
		{
			notVisitedNodes.push_back(loop1);
		}
	}
	if(notVisitedNodes.size() > 0)
	{
		return children[notVisitedNodes[rand() % notVisitedNodes.size()]];
	}

	double bestValue = children[0]->value + c * sqrt(log((double) visits) / children[0]->visits);;
	double val;
	for(int loop1 = 1; loop1 < childrenNumber; loop1++)
	{
		val = children[loop1]->value + c * sqrt(log((double) visits) / children[loop1]->visits);
		if(val > bestValue)
		{
			bestID = loop1;
			bestValue = val;
		}
	}
	return children[bestID];
}

MonteCarloTreeSearch::MonteCarloTreeSearch(IGameState * rootState, int _whoAskID, int algorithmIterations, float * coefMetric, bool delta)
{
	rootNode = new MonteCarloNode(rootState, NULL, -1);
	whoAskID = _whoAskID;

	MonteCarloNode * tempNode1;
	for(int loop1 = 0; loop1 < algorithmIterations; loop1++)
	{
		tempNode1 = rootNode;

		int maxDepth = loop1 / 1000 + 2;
		int depth = 0;
		while(tempNode1->Children() != NULL && depth < maxDepth)
		{
			tempNode1 = Selection(tempNode1);
			depth++;
		}

		if(depth != maxDepth)
			tempNode1->Expansion(whoAskID);

		GameStat stat = Simulation(tempNode1);

		if(delta)
		{
			float tempVals[COEF_COUNT];
			tempVals[COEF_LEADER_SWITCHES] = coefMetric[COEF_LEADER_SWITCHES] * stat.LeaderSwitches();
			tempVals[COEF_CREDIBILITY] = coefMetric[COEF_CREDIBILITY] * stat.Credibility();
			tempVals[COEF_RANDOMNESS] = coefMetric[COEF_RANDOMNESS] * stat.Randomness();
			tempVals[COEF_JUSTICE] = coefMetric[COEF_JUSTICE] * stat.Justice();
			tempVals[COEF_LEADER_TIME] = coefMetric[COEF_LEADER_TIME] * stat.LeaderTime();
			tempVals[COEF_AVG_STATUS_DIFFERENCE] = coefMetric[COEF_AVG_STATUS_DIFFERENCE] * stat.StatusDifference();
			tempVals[COEF_END_STATUS_DIFFERENCE] = coefMetric[COEF_END_STATUS_DIFFERENCE] * stat.EndRankDifference();
			tempVals[COEF_FREEDOM] = coefMetric[COEF_FREEDOM] * stat.Freedom();
			tempNode1->Backpropagation(tempVals);
		}
		else
		{
			float wSum = Utility::Inst()->WeightedMetrics(&stat, coefMetric);
			tempNode1->Backpropagation(wSum);
		}
	}
}

MonteCarloNode * MonteCarloTreeSearch::Selection(MonteCarloNode * tempNode)
{
	return tempNode->GetBestChild(whoAskID);
}

GameStat MonteCarloTreeSearch::Simulation(MonteCarloNode * tempNode)
{
	return tempNode->GameState()->GetGameStat();

	/*
	IGameState * tempState;
	tempState = nodeState->SimulateToTheEnd(whoAskID);
	int result = tempState->GetPlayerRank(whoAskID, 0);
	delete tempState;
	return result;*/

	/*
	int stateID;
	int depth = 0;
	int depthMax = 10;
	if(!nodeState->IsGameOver())
	{
		nodeState = nodeState->GetRandomNextState(whoAskID, &stateID);
		while(!nodeState->IsGameOver() && depth++ < depthMax)
		{
			tempState = nodeState;
			nodeState = nodeState->GetRandomNextState(whoAskID, &stateID);
			delete tempState;
		}
		int rank = nodeState->GetPlayerRank(whoAskID, whoAskID);
		delete nodeState;
		return rank;
	} else {
		return nodeState->GetPlayerRank(whoAskID, whoAskID);
	}*/
}

int MonteCarloTreeSearch::BestTurn()
{
	return rootNode->GetBestChildID();
}

MonteCarloTreeSearch::~MonteCarloTreeSearch(void)
{
	delete rootNode;
}
