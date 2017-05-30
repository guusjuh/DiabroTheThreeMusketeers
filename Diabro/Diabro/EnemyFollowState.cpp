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
	agent->walkToNextPoint();
}

void EnemyFollowState::Execute(BaseNpc* agent){
	//check if state transition is needed
	if (agent->getPosition().distance(GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->getPosition()) < agent->getCity()->scalar) {
		agent->stateMachine.setState("Attack");
	}
	//check if state transition is needed
	if (agent->getPosition().distance(GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->getPosition()) > agent->getCity()->scalar * 3) {
		agent->stateMachine.setState("AroundCenter");
	}

	if (agent->getPosition().distance(Ogre::Vector3(agent->getGoalPos().x, agent->getPosition().y, agent->getGoalPos().z)) < 50 || agent->getNextPosSize() == 0){
		int scale = GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->scalar;
		Ogre::Vector3 playerPos = GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->getPosition();
		Coordinate player = Coordinate(playerPos.x, playerPos.z);
		player = GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getGridPosition(player);
		playerPos.x = player.x;
		playerPos.z = player.z;
		agent->calculateAStar(playerPos);

		if (agent->getNextPosSize() > 0){
			agent->walkToNextPoint();
		}
	}
}

void EnemyFollowState::Exit(BaseNpc* agent){

}

void EnemyFollowState::Collide(BaseNpc* agent){
	if (agent->getNextPosSize() > 0)
	{
		agent->walkToNextPoint();
	}
	else
	{
		int scale = GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->scalar;
		Ogre::Vector3 playerPos = GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->getPosition();
		Coordinate player = Coordinate(playerPos.x, playerPos.z);
		player = GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getGridPosition(player);
		playerPos.x = player.x;
		playerPos.z = player.z;
		agent->calculateAStar(playerPos);
		agent->walkToNextPoint();
	}
}