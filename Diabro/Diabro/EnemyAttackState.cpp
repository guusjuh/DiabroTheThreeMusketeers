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
	if (agent->getPosition().distance(GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->getPosition()) <= agent->getAttackDistance()) {
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