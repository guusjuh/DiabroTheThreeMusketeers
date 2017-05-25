#include "Quest.h"
#include "GameManager.h"

/// <summary>
/// Initializes a new instance of the <see cref="Quest"/> class.
/// </summary>
/// <param name="strategy">The strategy.</param>
/// <param name="sourceNPC">The source NPC.</param>
/// <param name="healthReward">The health reward.</param>
Quest::Quest(Strategy strategy, BaseNpc* sourceNPC, int healthReward)
	: _strategy(strategy), _sourceNPC(sourceNPC), _healthReward(healthReward), _completed(false) {
}

/// <summary>
/// Initializes a new instance of the <see cref="Quest"/> class.
/// </summary>
Quest::Quest()
	: _strategy(), _sourceNPC(nullptr), _healthReward(0), _completed(false) { }

/// <summary>
/// Finalizes an instance of the <see cref="Quest"/> class.
/// </summary>
Quest::~Quest() {}

/// <summary>
/// Completes the current action and sets the next.
/// </summary>
void Quest::completeAction() {
	// complete the action
	_strategy.getCurrentAction()->complete();

	// if it's not the last
	if(_strategy.getCurrentAction() == &_strategy.getActionSequence()[_strategy.getActionSequence().size() - 1]) {
		completeQuest();
		return;
	} else {
		_strategy.increaseAction();
		return;
	}
		
}

/// <summary>
/// Completes the quest.
/// </summary>
void Quest::completeQuest() {
	// give rewards
	//TODO: also give the upgrade as reward
	GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->adjustHealth(_healthReward);

	//TODO: sound, ui, fancy stuff, hud text

	_completed = true;
}

