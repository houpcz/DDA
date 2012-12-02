#include "Human.h"

Human::~Human(void)
{
}

void Human::HumanTurn(int turn)
{
	isReady = true;
	myTurn = turn;

	emit ImReady();
}