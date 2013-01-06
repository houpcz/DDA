#ifndef _ISPECIFICSTAT_H_
#define _ISPECIFICSTAT_H_

#include <QString>

#ifndef NULL
#define NULL 0
#endif


class ISpecificStat
{
public:
	ISpecificStat(void);
	virtual ~ISpecificStat(void);
	virtual ISpecificStat * Plus(ISpecificStat * other) = 0;
	virtual ISpecificStat * CopyYourself() = 0;
	virtual void Reset() = 0;
	virtual QString Header() = 0;
	virtual QString Record() = 0;
};

#endif

