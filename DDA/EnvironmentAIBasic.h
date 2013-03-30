#ifndef _ENVIRONMENTAIBASIC_H_
#define _ENVIRONMENTAIBASIC_H_

#include "environmentai.h"
class EnvironmentAIBasic :
	public EnvironmentAI
{
Q_OBJECT

public:
	EnvironmentAIBasic(int _myID) : EnvironmentAI(_myID) {};
	virtual ~EnvironmentAIBasic(void);
	virtual bool Think();	
	QString GetAIName() { return QString("Random Environment"); };
	IPlayer * Factory(int playerID) { return new EnvironmentAIBasic(playerID); };
};

#endif