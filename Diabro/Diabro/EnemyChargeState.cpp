#include "EnemyChargeState.h"
#include "GameManager.h"

EnemyChargeState::EnemyChargeState()
{

}

EnemyChargeState::~EnemyChargeState()
{

}

void EnemyChargeState::Enter(BaseNpc* agent){
	Ogre::Vector3 playerPos = GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->getPosition();

	agent->walkTo(playerPos);
}

void EnemyChargeState::Execute(BaseNpc* agent){
	Ogre::Vector3 playerPos = GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->getPosition();

	//check if state transition is needed
	if (agent->getPosition().distance(playerPos) > 250) {
		agent->stateMachine.setState("FollowAStar");
		return;
	}
}

void EnemyChargeState::Exit(BaseNpc* agent){
}

void EnemyChargeState::Collide(BaseNpc* agent){
	agent->lightAttack();
}