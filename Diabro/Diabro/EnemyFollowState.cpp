#include "EnemyFollowState.h"
#include "GameManager.h"

EnemyFollowState::EnemyFollowState()
{

}

EnemyFollowState::~EnemyFollowState()
{

}

void EnemyFollowState::Enter(BaseNpc* agent){
	int scale = GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->scalar;
	Ogre::Vector3 playerPos = GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->getPosition();
	Coordinate player = Coordinate(playerPos.x, playerPos.z);
	player = GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getGridPosition(player);
	playerPos.x = player.x;
	playerPos.z = player.z;
	agent->calculateAStar(playerPos);
}

void EnemyFollowState::Execute(BaseNpc* agent){
	//check if state transition is needed
	if (agent->getPosition().distance(GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->getPosition()) < agent->_myCity->scalar / 2) {
		agent->stateMachine.setState("Attack");
	}

	if (agent->getPosition().distance(Ogre::Vector3(agent->goalPos.x, agent->getPosition().y, agent->goalPos.z)) < 50 || agent->nextPos.size() == 0){
		int scale = GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->scalar;
		Ogre::Vector3 playerPos = GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->getPosition();
		Coordinate player = Coordinate(playerPos.x, playerPos.z);
		player = GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getGridPosition(player);
		playerPos.x = player.x;
		playerPos.z = player.z;
		agent->calculateAStar(playerPos);

		if (agent->nextPos.size() > 0){
			agent->walkToNextPoint();
		}
	}
}

void EnemyFollowState::Exit(BaseNpc* agent){

}