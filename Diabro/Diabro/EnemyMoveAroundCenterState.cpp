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
	while (agent->getNextPosSize() == 0)
	{
		setNextPointList(agent);
	}
	agent->walkToNextPoint();
}

void EnemyMoveAroundCenterState::Execute(BaseNpc* agent){
	//check if state transition is needed
	if (agent->getPosition().distance(GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->getPosition()) < agent->getCity()->Scalar() * 2) {
		agent->stateMachine.setState("Follow");
	}
	if (agent->getDirVector() == Ogre::Vector3().ZERO){
		if (agent->getNextPosSize() == 0){
			while (agent->getNextPosSize() == 0)
			{
				setNextPointList(agent);
			}
			agent->walkToNextPoint();
		}
		else{
			agent->walkToNextPoint();
		}
	}

	if (agent->getPosition().distance(Ogre::Vector3(agent->getGoalPos().x, agent->getPosition().y, agent->getGoalPos().z)) < agent->getSpeed() / 2){
		if (agent->getNextPosSize() == 0){
			while (agent->getNextPosSize() == 0)
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
	if (agent->getNextPosSize() > 0)
	{
		agent->walkToNextPoint();
	}
	else
	{
		while (agent->getNextPosSize() == 0)
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
	Coordinate centerPos = agent->getCity()->getCenterTile();
	int scalar = agent->getCity()->Scalar();
	switch (cornerId){
	case 0:
		agent->calculateAStar(Ogre::Vector3((centerPos.x - 1) * City::gridScalar, 0, (centerPos.z - 1) * City::gridScalar));
		break;
	case 1:
		agent->calculateAStar(Ogre::Vector3((centerPos.x - 1) * City::gridScalar, 0, (centerPos.z + 1) * City::gridScalar));
		break;
	case 2:
		agent->calculateAStar(Ogre::Vector3((centerPos.x + 1) * City::gridScalar, 0, (centerPos.z + 1) * City::gridScalar));
		break;
	case 3:
		agent->calculateAStar(Ogre::Vector3((centerPos.x + 1) * City::gridScalar, 0, (centerPos.z - 1) * City::gridScalar));
		break;
	}
}