#include "EnemyAttackState.h"
#include "GameManager.h"

EnemyAttackState::EnemyAttackState()
{

}

EnemyAttackState::~EnemyAttackState()
{

}

void EnemyAttackState::Enter(BaseNpc* agent){

}

void EnemyAttackState::Execute(BaseNpc* agent){
	Ogre::Vector3 playerPos = GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->getPosition();
	
	if (agent->getPosition().distance(playerPos) <= agent->getAttackDistance()) {
		agent->lightAttack();
		agent->setDirVector(Ogre::Vector3().ZERO);
		return;
	}
	else{
		agent->stateMachine.setState("FollowDirect");
		return;
	}
}

void EnemyAttackState::Exit(BaseNpc* agent){

}