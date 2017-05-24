#pragma once
#include "Action.h"
#include "Strategy.h"
#include "Npc.h"

/// <summary>
/// A linear Quest instance.
/// </summary>
class Quest
{
public:
	Quest(Strategy strategy, Npc* sourceNPC, int healthReward);
	Quest();
	~Quest();

	void completeAction();
	void completeQuest();
	bool completed() { return _completed; }

private:
	Strategy _strategy;
	Action* _currrentAction;
	Npc* _sourceNPC;
	int _healthReward;
	bool _completed;
};

