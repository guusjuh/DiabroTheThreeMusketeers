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

	//check if state transition is needed
	if (agent->getPosition().distance(playerPos) > agent->getCity()->scalar) {
		agent->stateMachine.setState("Follow");
		return;
	}

	if (agent->getPosition().distance(playerPos) < agent->getAttackDistance()) {
		agent->lightAttack();
		agent->setDirVector(Ogre::Vector3().ZERO);
		return;
	}

	agent->walkTo(playerPos);
}

void EnemyAttackState::Exit(BaseNpc* agent){

}