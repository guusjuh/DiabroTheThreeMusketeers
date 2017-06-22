#include "EnemyMeleeAttackState.h"
#include "GameManager.h"

/// <summary>
/// Executes the attack state on its agent.
/// </summary>
/// <param name="agent">The agent.</param>
void EnemyMeleeAttackState::Execute(BaseNpc* agent) {
	// if the agent is close enough to perform it's attack
	if (agent->getPosition().distance(GameManager::getSingletonPtr()->getPlayer()->getPosition()) <= agent->getAttackDistance()) {
		
		// perform the attack
		agent->lightAttack();

		// make the agent stop moving
		agent->setDirVector(Ogre::Vector3().ZERO);

		// TODO: shouldn't the enemy leave the attack state too here?

		return;
	}

	// else, exit the attack state
	else{
		agent->stateMachine.setState("FollowDirect");
		return;
	}
}