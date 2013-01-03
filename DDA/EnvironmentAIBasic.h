#ifndef _ENVIRONMENTAIBASIC_H_
#define _ENVIRONMENTAIBASIC_H_

#include "ienvironmentai.h"
class EnvironmentAIBasic :
	public IEnvironmentAI
{
Q_OBJECT

public:
	EnvironmentAIBasic(int _myID) : IEnvironmentAI(_myID) {};
	virtual ~EnvironmentAIBasic(void);
	virtual bool Think();	
	QString GetAIName() { return QString("Random Environment"); };
};

#endif