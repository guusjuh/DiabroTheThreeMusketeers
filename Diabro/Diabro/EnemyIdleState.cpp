#include "EnemyIdleState.h"
#include "GameManager.h"

void EnemyIdleState::Execute(BaseNpc* agent) {
	agent->setDirVector(Ogre::Vector3::ZERO);

	// attack is curr on cooldown, cannot attack or walk
	if(!agent->onAttackCooldown()) {
		if (agent->getPosition().distance(GameManager::getSingletonPtr()->getPlayer()->getPosition()) < agent->getAttackDistance()) {
			agent->getMyNode()->lookAt(Ogre::Vector3(GameManager::getSingletonPtr()->getPlayer()->getPosition().x, 
				agent->getPosition().y, GameManager::getSingletonPtr()->getPlayer()->getPosition().z), 
				Ogre::Node::TS_WORLD, Ogre::Vector3::UNIT_X);

			agent->stateMachine.setState("Attack");
			return;
		}
		else {
			agent->stateMachine.setState("AroundCenter");
			return;
		}
	}
}	
