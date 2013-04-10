#ifndef _MONTECARLOTREESEARCH_H_
#define _MONTECARLOTREESEARCH_H_

#include <map>
#include "IGameState.h"
#include "EnvironmentAI.h"

class MonteCarloNode
{
private:
	int turnID;
	int visits;
	double value;
	IGameState * myState;
	int childrenNumber;
	float minValue[COEF_COUNT];
	float maxValue[COEF_COUNT];
	MonteCarloNode * parent;
	MonteCarloNode ** children;
public:
	MonteCarloNode(IGameState * _myState, MonteCarloNode * _parent, int _turnID);
	~MonteCarloNode();
	MonteCarloNode * GetBestChild(int whoAskID);
	MonteCarloNode * GetRandomChild();
	void Expansion(int whoAskID);
	void Backpropagation(float * values);
	void Backpropagation(double reward);

	MonteCarloNode ** Children() { return children; }
	int GetBestChildID();
	IGameState * GameState() { return myState; }
};

class MonteCarloTreeSearch
{
private:
	MonteCarloNode * rootNode;
	int whoAskID;

	MonteCarloNode * Selection(MonteCarloNode *);
	virtual GameStat Simulation(MonteCarloNode *);
public:
	MonteCarloTreeSearch(IGameState * rootState, int whoAskID, int algorithmIterations, float * coefMetric, bool delta);
	virtual ~MonteCarloTreeSearch(void);
	int BestTurn();
};

#endif