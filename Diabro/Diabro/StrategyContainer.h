#ifndef STRATEGYCONTAINER_H
#define STRATEGYCONTAINER_H

#include "GenericContainer.h"
#include "Strategy.h"

/// <summary>
/// A container to store the strategies to be used in quests. 
/// </summary>
/// <seealso cref="GenericContainer{Strategy}" />
class StrategyContainer : public GenericContainer<Strategy> {
public:
	StrategyContainer();
	~StrategyContainer();

	void readFromXML();
};

#endif
