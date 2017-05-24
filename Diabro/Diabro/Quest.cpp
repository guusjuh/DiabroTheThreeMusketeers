#include "Quest.h"
#include "GameManager.h"

/// <summary>
/// Initializes a new instance of the <see cref="Quest"/> class.
/// </summary>
/// <param name="strategy">The strategy.</param>
/// <param name="sourceNPC">The source NPC.</param>
/// <param name="healthReward">The health reward.</param>
Quest::Quest(Strategy strategy, Npc* sourceNPC, int healthReward)
	: _strategy(strategy), _sourceNPC(sourceNPC), _healthReward(healthReward) {
	_currrentAction = _strategy.getActionSequence()[0];
}

/// <summary>
/// Initializes a new instance of the <see cref="Quest"/> class.
/// </summary>
Quest::Quest()
	: _strategy(), _sourceNPC(nullptr), _healthReward(0), _currrentAction(nullptr) { }

/// <summary>
/// Finalizes an instance of the <see cref="Quest"/> class.
/// </summary>
Quest::~Quest() { }

/// <summary>
/// Completes the current action and sets the next.
/// </summary>
void Quest::completeAction() {
	// find which action is the current
	for (int i = 0; i < _strategy.getActionSequence().size(); ++i) {

		// if we found the current
		if(_currrentAction == _strategy.getActionSequence()[i]) {
			
			// complete the action
			_currrentAction->complete();

			// if it's not the last
			if(i != _strategy.getActionSequence().size() - 1) {
				completeQuest();
			} else {
				_currrentAction = _strategy.getActionSequence()[i + 1];
				return;
			}
		}
	}
	
}

/// <summary>
/// Completes the quest.
/// </summary>
void Quest::completeQuest() {
	// give rewards
	//TODO: also give the upgrade as reward
	GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->adjustHealth(_healthReward);

	_completed = true;
}

