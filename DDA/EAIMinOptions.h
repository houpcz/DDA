#ifndef _EAIMINOPTIONS_H_
#define _EAIMINOPTIONS_H_

#include "environmentai.h"
class EAIMinOptions :
	public EnvironmentAI
{
Q_OBJECT

public:
	EAIMinOptions(int _myID) : EnvironmentAI(_myID) {};
	virtual ~EAIMinOptions(void);
	virtual bool Think();	
	QString GetAIName() { return QString("Min Options"); };
	IPlayer * Factory(int playerID) { return new EAIMinOptions(playerID); };
};

#endif
