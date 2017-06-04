#include "EnemyFollowAStarState.h"
#include "GameManager.h"

EnemyFollowAStarState::EnemyFollowAStarState()
{

}

EnemyFollowAStarState::~EnemyFollowAStarState()
{

}

void EnemyFollowAStarState::Enter(BaseNpc* agent){
	Ogre::Vector3 playerPos = GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->getPosition();
	Coordinate player = Coordinate(playerPos.x, playerPos.z);
	player = GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getCollisionGridPosition(player);
	playerPos.x = player.x;
	playerPos.z = player.z;
	agent->calculateAStar(playerPos);
	agent->walkToNextPoint();
}

void EnemyFollowAStarState::Execute(BaseNpc* agent){
	//check if state transition is needed
	if (agent->getPosition().distance(GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->getPosition()) < Zone::scalar / 2.0f) {
		agent->stateMachine.setState("FollowDirect");
	}
	//check if state transition is needed
	if (agent->getPosition().distance(GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->getPosition()) > agent->getNoticeDistance()) {
		agent->stateMachine.setState("AroundCenter");
	}

	if (agent->getPosition().distance(Ogre::Vector3(agent->getGoalPos().x, agent->getPosition().y, agent->getGoalPos().z)) < agent->getSpeed() / 2.0f || agent->getNextPosSize() == 0){
		int scale = Zone::scalar;
		Ogre::Vector3 playerPos = GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->getPosition();
		Coordinate player = Coordinate(playerPos.x, playerPos.z);
		player = GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getCollisionGridPosition(player);
		playerPos.x = player.x;
		playerPos.z = player.z;
		agent->calculateAStar(playerPos);

		if (agent->getNextPosSize() > 0){
			agent->walkToNextPoint();
		}
	}
}

void EnemyFollowAStarState::Exit(BaseNpc* agent){

}

void EnemyFollowAStarState::Collide(BaseNpc* agent){/*
	if (agent->getNextPosSize() > 0)
	{
		agent->walkToNextPoint();
	}
	else
	{
		int scale = Zone::scalar;
		Ogre::Vector3 playerPos = GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->getPosition();
		Coordinate player = Coordinate(playerPos.x, playerPos.z);
		player = GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getCollisionGridPosition(player);
		playerPos.x = player.x;
		playerPos.z = player.z;
		agent->calculateAStar(playerPos);
		agent->walkToNextPoint();
	}*/
	agent->walkToNeighbour();
}