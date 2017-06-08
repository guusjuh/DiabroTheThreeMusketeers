#include "Quest.h"
#include "GameManager.h"
#include "Debug.h"

/// <summary>
/// Initializes a new instance of the <see cref="Quest"/> class.
/// </summary>
/// <param name="strategy">The strategy.</param>
/// <param name="sourceNPC">The source NPC.</param>
/// <param name="healthReward">The health reward.</param>
Quest::Quest(Strategy strategy, BaseNpc* sourceNPC, int healthReward, PlayerUpgradeType upgrade)
	: _strategy(strategy), _sourceNPC(sourceNPC), _healthReward(healthReward), _upgradeReward(upgrade), _completed(false), _initialized(false){
}

/// <summary>
/// Initializes a new instance of the <see cref="Quest"/> class.
/// </summary>
Quest::Quest()
	: _strategy(), _sourceNPC(nullptr), _healthReward(0), _upgradeReward(), _completed(false), _initialized(false) { }

/// <summary>
/// Finalizes an instance of the <see cref="Quest"/> class.
/// </summary>
Quest::~Quest() {}

/// <summary>
/// Starts the quest.
/// </summary>
void Quest::start() {	
	// set the first action as current
	_strategy.increaseAction();

	Debug("\tQ: active action is set to ", _strategy._currentAction);

	// set the target for player/ui
	_currentTarget = _strategy._actionSequence[_strategy._currentAction].getTarget();

	Debug("\tQ: I have set a target for the locator.");

	// notify the ui
	GameManager::getSingletonPtr()->getUIManager()->setQuestOn(true);

	_initialized = true;
}

void Quest::update() {
	if (!_initialized) return;

	_strategy._actionSequence[_strategy._currentAction].update();

	if(_strategy._actionSequence[_strategy._currentAction].completed()) {
		if(!_strategy.increaseAction()) {
			end();
			return;
		} else {
			_currentTarget = _strategy._actionSequence[_strategy._currentAction].getTarget();
		}
	}

	updateLocatorPos();
}

/// <summary>
/// Ends the quest, assigns rewards.
/// </summary>
void Quest::end() {
	GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->upgradeEquipment(_upgradeReward);
	GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->adjustHealth(-_healthReward);

	GameManager::getSingletonPtr()->getUIManager()->setQuestOn(false);
	Debug("\tQuest: I am complete!");
	_completed = true;
}

/// <summary>
/// Abondons the quest.
/// </summary>
void Quest::abondon() {
	GameManager::getSingletonPtr()->getUIManager()->setQuestOn(false);
}

/// <summary>
/// Updates the locator position.
/// </summary>
void Quest::updateLocatorPos() {
	// update the locator
	GameManager::getSingletonPtr()->getUIManager()->setQuestTarget(_currentTarget->getQuestPosition());
}

void Quest::sendMsg(IQuestContent* sender, std::string msg) {
	_strategy._actionSequence[_strategy._currentAction].sendMsg(sender, msg);
}
