#include "EnemyMoveAroundCenterState.h"
#include "GameManager.h"

EnemyMoveAroundCenterState::EnemyMoveAroundCenterState()
{

}

EnemyMoveAroundCenterState::~EnemyMoveAroundCenterState()
{

}

void EnemyMoveAroundCenterState::Enter(BaseNpc* agent){
	setNextPointList(agent);
	agent->walkToNextPoint();
}

void EnemyMoveAroundCenterState::Execute(BaseNpc* agent){
	//check if state transition is needed
	if (agent->getPosition().distance(GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->getPosition()) < agent->_myCity->scalar * 2) {
		agent->stateMachine.setState("Follow");
	}

	if (agent->getPosition().distance(Ogre::Vector3(agent->goalPos.x, agent->getPosition().y, agent->goalPos.z)) < 50){
		if (agent->nextPos.size() == 0){
			setNextPointList(agent);
			agent->walkToNextPoint();
		}
		else{
			agent->walkToNextPoint();
		}
	}

}

void EnemyMoveAroundCenterState::Exit(BaseNpc* agent){

}

void EnemyMoveAroundCenterState::setNextPointList(BaseNpc* agent){
	agent->nextPos.clear();
	Coordinate centerPos = agent->_myCity->getCenterTile();
	int scalar = agent->_myCity->scalar;
	agent->nextPos.push_back(Coordinate((centerPos.x - 1) * scalar, (centerPos.z - 1) * scalar));
	agent->nextPos.push_back(Coordinate((centerPos.x - 1) * scalar, (centerPos.z + 1) * scalar));
	agent->nextPos.push_back(Coordinate((centerPos.x + 1) * scalar, (centerPos.z + 1) * scalar));
	agent->nextPos.push_back(Coordinate((centerPos.x + 1) * scalar, (centerPos.z - 1) * scalar));
}