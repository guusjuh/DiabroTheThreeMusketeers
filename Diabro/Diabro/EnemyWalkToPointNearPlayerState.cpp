#include "EnemyWalkToPointNearPlayerState.h"
#include "GameManager.h"

EnemyWalkToPointNearPlayerState::EnemyWalkToPointNearPlayerState()
{

}

EnemyWalkToPointNearPlayerState::~EnemyWalkToPointNearPlayerState()
{
}

void EnemyWalkToPointNearPlayerState::Enter(BaseNpc* agent){
	calculateRelativePoint();
}

void EnemyWalkToPointNearPlayerState::Execute(BaseNpc* agent){
	Ogre::Vector3 playerPos = GameManager::getSingletonPtr()->getPlayer()->getPosition() + relative;

	//check if state transition is needed
	if (agent->getPosition().distance(playerPos) > 250) {
		agent->stateMachine.setState("FollowAStar");
		return;
	}

	if (agent->getPosition().distance(playerPos) < agent->getAttackDistance()) {
		agent->stateMachine.setState("Attack");
		return;
	}

	agent->walkTo(playerPos);
}

void EnemyWalkToPointNearPlayerState::Exit(BaseNpc* agent){
}

void EnemyWalkToPointNearPlayerState::calculateRelativePoint(){
	relative = Ogre::Vector3(200, 0, 0);
	int rndDegree = GameManager::getSingletonPtr()->getRandomInRange(0, 360);
	Ogre::Quaternion rot(Ogre::Degree(rndDegree), Ogre::Vector3::UNIT_Y);
	relative = rot * relative;
}