#include "EMonteCarlo.h"
#include "IGame.h"
#include "Utility.h"

using namespace std;

EMonteCarlo::EMonteCarlo(int _myID) : EnvironmentAI(_myID)
{
	random_device randomDevice;
    generator = new mt19937(randomDevice());
}

EMonteCarlo::~EMonteCarlo(void)
{
	delete generator;
}


bool EMonteCarlo::Think()
{
	return true;
}
