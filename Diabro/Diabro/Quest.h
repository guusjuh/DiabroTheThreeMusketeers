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
	friend class DialogManager;

public:
	Quest(Strategy strategy, BaseNpc* sourceNPC, int healthReward, PlayerUpgradeType upgrade);
	Quest();
	~Quest();

	void start();
	void update();
	void end();
	void abandon();

	bool isAbstract() { return _strategy.isAbstract(); }

	bool completed() { return _completed; }

	void sendMsg(IQuestContent* sender, std::string msg);

	BaseNpc* getSourceNpc() { return _sourceNPC; }

	std::string getCurrentInstruction() {
		if (_strategy._currentAction == -1) return "";
		return _strategy._actionSequence[_strategy._currentAction].getInstruction();
	}

	int getCurrentAction() { return _strategy._currentAction; }
	Strategy* getStrategy() { return &_strategy; }

	bool isContentInAction(IQuestContent* contentToMatch, int actionNr) {
		if (actionNr < 0 || actionNr >= _strategy.getActionSequence().size()) return false;
		
		if(_strategy.getActionSequence()[actionNr].contentContains(contentToMatch) != -1) {
			return true;
		}

		return false;
	}

	bool isContentInQuest(IQuestContent* contentToMatch) {
		for (int i = 0; i < _strategy.getActionSequence().size(); ++i) {
			if (_strategy.getActionSequence()[i].contentContains(contentToMatch) != -1) {
				return true;
			}
		}

		return false;
	}

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