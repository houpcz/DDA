#ifndef _ENVIRONMENTAIBASIC_H_
#define _ENVIRONMENTAIBASIC_H_

#include "ienvironmentai.h"
class EnvironmentAIBasic :
	public IEnvironmentAI
{
Q_OBJECT

public:
	EnvironmentAIBasic(void);
	~EnvironmentAIBasic(void);
	virtual void Think();	
};

#endif