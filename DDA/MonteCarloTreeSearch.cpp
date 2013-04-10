#include "MonteCarloTreeSearch.h"
#include <vector>
#include "IPlayer.h"
#include "Utility.h"

using namespace std;

MonteCarloNode::MonteCarloNode(IGameState * _myState, MonteCarloNode * _parent)
{
	parent = _parent;
	myState = _myState;
	visits = 0;
	value = 0.0;
	childrenNumber = 0;
	children = NULL;
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

	IGameState ** nextStates = myState->GetNextStates(whoAskID, &childrenNumber);

	children = new MonteCarloNode*[childrenNumber];

	for(int loop1 = 0; loop1 < childrenNumber; loop1++)
	{
		children[loop1] = new MonteCarloNode(nextStates[loop1], this);
	}
	delete [] nextStates;
}

int MonteCarloNode::GetBestChildID(int level)
{
	vector<valueDIndex> scores;
	for(int loop1 = 0; loop1 < childrenNumber; loop1++)
	{
		scores.push_back(valueDIndex(children[loop1]->value, loop1));
	}
	sort(scores.begin(), scores.end(), comparatorD);
	return scores[Utility::Inst()->GetTurnIDByLevel(scores.size(), level)].second;
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

MonteCarloTreeSearch::MonteCarloTreeSearch(IGameState * rootState, int _whoAskID, int algorithmIterations)
{
	rootNode = new MonteCarloNode(rootState, NULL);
	whoAskID = _whoAskID;

	MonteCarloNode * tempNode1;
	for(int loop1 = 0; loop1 < algorithmIterations; loop1++)
	{
		tempNode1 = rootNode;

		int maxDepth = loop1 / 2000 + 3;
		int depth = 0;
		while(tempNode1->Children() != NULL && depth < maxDepth)
		{
			tempNode1 = Selection(tempNode1);
			depth++;
		}

		if(depth != maxDepth)
			tempNode1->Expansion(whoAskID);

		double reward = Simulation(tempNode1);
		tempNode1->Backpropagation(reward);
	}
}

MonteCarloNode * MonteCarloTreeSearch::Selection(MonteCarloNode * tempNode)
{
	if(tempNode->GameState()->GetActivePlayerID() == 0)
	{
		return tempNode->GetRandomChild();
	} else {
		return tempNode->GetBestChild(whoAskID);
	}
}

double MonteCarloTreeSearch::Simulation(MonteCarloNode * tempNode)
{
	return tempNode->GameState()->GetPlayerRank(whoAskID, 0);

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

int MonteCarloTreeSearch::BestTurn(int level)
{
	return rootNode->GetBestChildID(level);
}

MonteCarloTreeSearch::~MonteCarloTreeSearch(void)
{
	delete rootNode;
}
