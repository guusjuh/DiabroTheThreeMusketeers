#ifndef QUEST_H
#define QUEST_H

#include "Action.h"
#include "Strategy.h"
#include "BaseNpc.h"
#include "PlayerEquipment.h"

/// <summary>
/// A linear Quest instance.
/// </summary>
class Quest
{
	friend class QuestGenerator;
public:
	Quest(Strategy strategy, BaseNpc* sourceNPC, int healthReward, PlayerUpgradeType upgrade);
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
	PlayerUpgradeType _upgradeReward;

	bool _completed;
};

#endif