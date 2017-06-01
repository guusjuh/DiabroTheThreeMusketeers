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
	friend class QuestManager;

public:
	Quest(Strategy strategy, BaseNpc* sourceNPC, int healthReward, PlayerUpgradeType upgrade);
	Quest();
	~Quest();

	void start();
	void update();
	void end();
	void abondon();

	bool isAbstract() { return _strategy.isAbstract(); }

	bool completed() { return _completed; }

	void sendMsg(std::string msg);

private:
	Strategy _strategy;
	BaseNpc* _sourceNPC;

	int _healthReward;
	PlayerUpgradeType _upgradeReward;

	IQuestContent* _currentTarget;

	bool _completed;
	bool _initialized;

	void updateLocatorPos();
};

#endif