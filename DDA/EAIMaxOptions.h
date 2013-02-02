#ifndef _EAIMAXOPTIONS_H_
#define _EAIMAXOPTIONS_H_

#include "ienvironmentai.h"
class EAIMaxOptions :
	public IEnvironmentAI
{
Q_OBJECT

public:
	EAIMaxOptions(int _myID) : IEnvironmentAI(_myID) {};
	virtual ~EAIMaxOptions(void);
	virtual bool Think();	
	QString GetAIName() { return QString("Max Options"); };
	IPlayer * Factory(int playerID) { return new EAIMaxOptions(playerID); };
};

#endif
