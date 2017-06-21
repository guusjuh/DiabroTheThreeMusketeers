#include "EnemyChargeState.h"
#include "GameManager.h"

EnemyChargeState::EnemyChargeState()
{

}

EnemyChargeState::~EnemyChargeState()
{

}

void EnemyChargeState::Enter(BaseNpc* agent){
	timer = 1.0f;
	agent->setDirVector(Ogre::Vector3(0, 0, 0));
}

void EnemyChargeState::Execute(BaseNpc* agent){
	Ogre::Vector3 playerPos = GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->getPosition();

	timer -= agent->timePast;
	if (timer < 0){
		timer = 5.0f;
		agent->walkTo(playerPos);
	}

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
	agent->setDirVector(Ogre::Vector3(0, 0, 0));
}