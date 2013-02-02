#ifndef _EAIMINOPTIONS_H_
#define _EAIMINOPTIONS_H_

#include "ienvironmentai.h"
class EAIMinOptions :
	public IEnvironmentAI
{
Q_OBJECT

public:
	EAIMinOptions(int _myID) : IEnvironmentAI(_myID) {};
	virtual ~EAIMinOptions(void);
	virtual bool Think();	
	QString GetAIName() { return QString("Min Options"); };
	IPlayer * Factory(int playerID) { return new EAIMinOptions(playerID); };
};

#endif
