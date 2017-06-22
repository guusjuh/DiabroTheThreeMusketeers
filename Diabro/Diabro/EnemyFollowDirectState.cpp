#include "EnemyFollowDirectState.h"
#include "GameManager.h"

/// <summary>
/// Executes this state.
/// </summary>
/// <param name="agent">The agent.</param>
void EnemyFollowDirectState::Execute(BaseNpc* agent) {
	 _playerPos = GameManager::getSingletonPtr()->getPlayer()->getPosition();

	// if the enemy is too far away to be in this state, go back to following the player with A*
	if (agent->getPosition().distance(_playerPos) >  Zone::scalar / 2.0f) {
		agent->stateMachine.setState("FollowAStar");
		return;
	}
	// else if, the enemy is close enough to attack
	else if (agent->getPosition().distance(_playerPos) < agent->getAttackDistance()) {
		agent->stateMachine.setState("Attack");
		return;
	}

	// make the agent walk to the player
	agent->walkTo(_playerPos);
}
