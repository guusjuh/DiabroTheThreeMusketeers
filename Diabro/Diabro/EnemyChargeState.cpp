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
}

void EnemyChargeState::Execute(BaseNpc* agent){
	Ogre::Vector3 playerPos = GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->getPosition();

	agent->setDirVector(Ogre::Vector3(0, 0, 0));
	agent->setGoalPos(Coordinate(playerPos.x, playerPos.z));

	timer -= GameManager::getSingletonPtr()->getDeltaTime();
	if (timer < 0){
		//agent->stateMachine.setState("ChargeAttack");
	}

	//check if state transition is needed
	if (agent->getPosition().distance(playerPos) > Zone::scalar) {
		agent->stateMachine.setState("FollowAStar");
		return;
	}
}

void EnemyChargeState::Exit(BaseNpc* agent){
}