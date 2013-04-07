#ifndef _MONTECARLOTREESEARCH_H_
#define _MONTECARLOTREESEARCH_H_

#include <map>
#include "IGameState.h"

class MonteCarloNode
{
private:
	int visits;
	double value;
	IGameState * myState;
	int childrenNumber;
	MonteCarloNode * parent;
	MonteCarloNode ** children;
public:
	MonteCarloNode(IGameState * _myState, MonteCarloNode * _parent);
	~MonteCarloNode();
	MonteCarloNode * GetBestChild(int whoAskID);
	MonteCarloNode * GetRandomChild();
	void Expansion(int whoAskID);
	void Backpropagation(double reward);

	MonteCarloNode ** Children() { return children; }
	int GetBestChildID(int level);
	IGameState * GameState() { return myState; }
};

class MonteCarloTreeSearch
{
private:
	MonteCarloNode * rootNode;
	int whoAskID;

	MonteCarloNode * Selection(MonteCarloNode *);
	double Simulation(MonteCarloNode *);
public:
	MonteCarloTreeSearch(IGameState * rootState, int whoAskID, int algorithmIterations);
	~MonteCarloTreeSearch(void);
	int BestTurn(int level);
};

#endif