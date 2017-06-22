#include "EnemyMoveAroundCenterState.h"
#include "GameManager.h"

/// <summary>
/// Enters this state.
/// </summary>
/// <param name="agent">The agent.</param>
void EnemyMoveAroundCenterState::Enter(BaseNpc* agent) {
	if(nextState == "") {
		nextState = "FollowAStar";
	}

	// start with a negative nr. to indicate the agent doesnt have any yet
	_cornerId = -1;
	
	// set the next route with A* based on it's current position
	setNextRoute(agent);

	// as long as the agent doesn't have a route to walk, find a new one
	while (agent->getNextPosSize() == 0)
	{
		setNextRoute(agent);
	}

	// start walking to the first point 
	agent->walkToNextPoint();
}

/// <summary>
/// Executes this state.
/// </summary>
/// <param name="agent">The agent.</param>
void EnemyMoveAroundCenterState::Execute(BaseNpc* agent) {
	// if the player came too close, start following him
	if (agent->getPosition().distance(GameManager::getSingletonPtr()->getPlayer()->getPosition()) < agent->getNoticeDistance()) {
		agent->stateMachine.setState(nextState);
		return;
	}

	// if agent is colliding
	if (agent->getDirVector() == Ogre::Vector3().ZERO){
		if (agent->getNextPosSize() == 0){
			while (agent->getNextPosSize() == 0)
			{
				setNextRoute(agent);
			}
			agent->walkToNextPoint();
		}
		else{
			agent->walkToNextPoint();
		}
	}

	// if agent got to it's goal position
	if (agent->getPosition().distance(Ogre::Vector3(agent->getGoalPos().x, agent->getPosition().y, agent->getGoalPos().z)) < agent->getSpeed() / 2){
		if (agent->getNextPosSize() == 0){
			while (agent->getNextPosSize() == 0)
			{
				setNextRoute(agent);
			}
			agent->walkToNextPoint();
		}
		else{
			agent->walkToNextPoint();
		}
	}
}

/// <summary>
/// Sets the next point list.
/// </summary>
/// <param name="agent">The agent.</param>
void EnemyMoveAroundCenterState::setNextRoute(BaseNpc* agent) {
	_cornerId++;
	if (_cornerId>3)
		_cornerId = -1;
	Coordinate centerPos = agent->getCity()->getCenterTile();
	int scalar = agent->getCity()->Scalar();
	switch (_cornerId){
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