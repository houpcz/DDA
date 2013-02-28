#include "MonteCarloTreeSearch.h"
#include <vector>

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
	/*
	if(childrenNumber == 0)
	{
		childrenNumber = 0;
		parent->Expansion(whoAskID);
		nextStates = myState->GetNextStates(whoAskID, &childrenNumber);
	}
	*/

	children = new MonteCarloNode*[childrenNumber];

	for(int loop1 = 0; loop1 < childrenNumber; loop1++)
	{
		children[loop1] = new MonteCarloNode(nextStates[loop1], this);
	}
	delete [] nextStates;
}

int MonteCarloNode::GetBestChildID()
{
	int bestID = 0;
	double bestVal = children[0]->value;
	for(int loop1 = 1; loop1 < childrenNumber; loop1++)
	{
		if(children[loop1]->value > bestVal)
		{
			bestID = loop1;
			bestVal = children[loop1]->value;
		}
	}
	return bestID;
}

void MonteCarloNode::Backpropagation(double reward)
{
	visits++;
	value += 1.0f / visits * (reward - value);
	

	if(parent != NULL)
	{
		if(myState->GetActivePlayerID() == 0)
			parent->Backpropagation(reward);
		else
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
			sumProb += GameState()->GetNextStateProb(whoAskID, loop1);
		}
	}
	if(notVisitedNodes.size() > 0)
	{
		double rndNumber = rand() / (double) RAND_MAX;
		double tempProb = 0.0;
		for(int loop1 = 0; loop1 < notVisitedNodes.size(); loop1++)
		{
			tempProb += GameState()->GetNextStateProb(whoAskID, notVisitedNodes[loop1]) / sumProb;
			if(rndNumber < tempProb)
			{
				return children[notVisitedNodes[loop1]];
			}
		}
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

		while(tempNode1->Children() != NULL)
		{
			tempNode1 = Selection(tempNode1);
		}

		tempNode1->Expansion(whoAskID);
		double reward = Simulation(tempNode1);
		tempNode1->Backpropagation(reward);
	}
}

MonteCarloNode * MonteCarloTreeSearch::Selection(MonteCarloNode * tempNode)
{
	if(tempNode->GameState()->GetActivePlayerID() == 0 && whoAskID != 0)
	{
		return tempNode->GetRandomChild();
	} else {
		return tempNode->GetBestChild(whoAskID);
	}
}

double MonteCarloTreeSearch::Simulation(MonteCarloNode * tempNode)
{
	IGameState * nodeState = tempNode->GameState();
	IGameState * tempState;
	int stateID;

	if(!nodeState->IsGameOver())
	{
		nodeState = nodeState->GetRandomNextState(whoAskID, &stateID);
		while(!nodeState->IsGameOver())
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
	}
}

int MonteCarloTreeSearch::BestTurn()
{
	return rootNode->GetBestChildID();
}

MonteCarloTreeSearch::~MonteCarloTreeSearch(void)
{
	delete rootNode;
}
