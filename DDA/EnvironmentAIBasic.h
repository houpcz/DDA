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
	virtual bool Think();	
	QString GetAINAme() { return QString("Random Environment"); };
};

#endif