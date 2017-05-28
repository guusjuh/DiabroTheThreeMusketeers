#ifndef QUEST_H
#define QUEST_H

#include "Action.h"
#include "Strategy.h"
#include "BaseNpc.h"

/// <summary>
/// A linear Quest instance.
/// </summary>
class Quest
{
	friend class QuestGenerator;
public:
	Quest(Strategy strategy, BaseNpc* sourceNPC, int healthReward);
	Quest();
	~Quest();

	void completeAction();
	void completeQuest();

	bool isAbstract() { return _strategy.isAbstract(); }

	bool completed() { return _completed; }

private:
	Strategy _strategy;
	BaseNpc* _sourceNPC;
	int _healthReward;

	bool _completed;
};

#endif