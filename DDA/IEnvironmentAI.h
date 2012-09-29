#ifndef _IENVIRONMENTAI_H_
#define _IENVIRONMENTAI_H_

#include "IPlayer.h"

class IEnvironmentAI : public IPlayer
{
Q_OBJECT

public :
	virtual void Think() = 0;
};

#endif