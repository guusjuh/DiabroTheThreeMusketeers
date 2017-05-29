#include "EnemyMoveAroundCenterState.h"
#include "GameManager.h"

EnemyMoveAroundCenterState::EnemyMoveAroundCenterState()
{

}

EnemyMoveAroundCenterState::~EnemyMoveAroundCenterState()
{

}

void EnemyMoveAroundCenterState::Enter(BaseNpc* agent){
	cornerId = -1;
	setNextPointList(agent);
	while (agent->nextPos.size() == 0)
	{
		setNextPointList(agent);
	}
	agent->walkToNextPoint();
}

void EnemyMoveAroundCenterState::Execute(BaseNpc* agent){
	//check if state transition is needed
	if (agent->getPosition().distance(GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->getPosition()) < agent->_myCity->scalar * 2) {
		agent->stateMachine.setState("Follow");
	}
	if (agent->getDirVector() == Ogre::Vector3().ZERO){
		if (agent->nextPos.size() == 0){
			while (agent->nextPos.size() == 0)
			{
				setNextPointList(agent);
			}
			agent->walkToNextPoint();
		}
		else{
			agent->walkToNextPoint();
		}
	}

	if (agent->getPosition().distance(Ogre::Vector3(agent->goalPos.x, agent->getPosition().y, agent->goalPos.z)) < 50){
		if (agent->nextPos.size() == 0){
			while (agent->nextPos.size() == 0)
			{
				setNextPointList(agent);
			}
			agent->walkToNextPoint();
		}
		else{
			agent->walkToNextPoint();
		}
	}

}

void EnemyMoveAroundCenterState::Exit(BaseNpc* agent){

}

void EnemyMoveAroundCenterState::Collide(BaseNpc* agent){
	if (agent->nextPos.size() > 0)
	{
		agent->walkToNextPoint();
	}
	else
	{
		while (agent->nextPos.size() == 0)
		{
			setNextPointList(agent);
		}
		agent->walkToNextPoint();
	}
}

void EnemyMoveAroundCenterState::setNextPointList(BaseNpc* agent){
	cornerId++;
	if (cornerId>3)
		cornerId = -1;
	agent->nextPos.clear();
	Coordinate centerPos = agent->_myCity->getCenterTile();
	int scalar = agent->_myCity->scalar;
	switch (cornerId){
	case 0:
		agent->calculateAStar(Ogre::Vector3(centerPos.x - 1, 0, centerPos.z - 1));
		break;
	case 1:
		agent->calculateAStar(Ogre::Vector3(centerPos.x - 1, 0, centerPos.z + 1));
		break;
	case 2:
		agent->calculateAStar(Ogre::Vector3(centerPos.x + 1, 0, centerPos.z + 1));
		break;
	case 3:
		agent->calculateAStar(Ogre::Vector3(centerPos.x + 1, 0, centerPos.z - 1));
		break;
	}
}