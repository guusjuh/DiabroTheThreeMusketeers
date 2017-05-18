#ifndef STRATEGYCONTAINER_H
#define STRATEGYCONTAINER_H

#include "GenericContainer.h"
#include "Strategy.h"

class StrategyContainer : GenericContainer<Strategy> {
public:
	StrategyContainer()
	{
		readFromXML();
	}
	~StrategyContainer() {
		GenericContainer<Strategy>::~GenericContainer();
	}

	void readFromXML();
};

#endif
