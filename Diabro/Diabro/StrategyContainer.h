#ifndef STRATEGYCONTAINER_H
#define STRATEGYCONTAINER_H

#include "GenericContainer.h"
#include "Strategy.h"

class StrategyContainer : public GenericContainer<Strategy> {
public:
	StrategyContainer();
	~StrategyContainer();

	void readFromXML();
};

#endif
