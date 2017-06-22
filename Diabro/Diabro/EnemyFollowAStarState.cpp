#include "EnemyFollowAStarState.h"
#include "GameManager.h"

/// <summary>
/// Enters this state.
/// </summary>
/// <param name="agent">The agent.</param>
void EnemyFollowAStarState::Enter(BaseNpc* agent) {
	// catch the player position in grid position
	_targetPos = getPlayerAsTarget();

	// calculate the A* route
	agent->calculateAStarWeighted(_targetPos);

	// walk to the first point of the calculated route
	agent->walkToNextPoint();
}

/// <summary>
/// Executes this state.
/// </summary>
/// <param name="agent">The agent.</param>
void EnemyFollowAStarState::Execute(BaseNpc* agent) {
	// if the agent is close enough, set the state to following him directly
	if (agent->getPosition().distance(GameManager::getSingletonPtr()->getPlayer()->getPosition()) < Zone::scalar / 2.0f && agent->_rotationType == 0) {
		agent->stateMachine.setState("FollowDirect");
	}
	else if (agent->getPosition().distance(GameManager::getSingletonPtr()->getPlayer()->getPosition()) < Zone::scalar && agent->_rotationType == 1){
		agent->stateMachine.setState("Charge");
	}

	// else if the agent is too far, start moving around the building in it's hometown again
	else if (agent->getPosition().distance(GameManager::getSingletonPtr()->getPlayer()->getPosition()) > agent->getNoticeDistance()) {
		agent->stateMachine.setState("AroundCenter");
	}

	// else if the agent reaced its goal in the A* route
	else if (agent->getPosition().distance(Ogre::Vector3(agent->getGoalPos().x, agent->getPosition().y, agent->getGoalPos().z)) < agent->getSpeed() / 2.0f) {
		// if it's the last goal position, calculate a new route
		if(agent->getNextPosSize() == 0) {
			_targetPos = getPlayerAsTarget();
			agent->calculateAStarWeighted(_targetPos);
		} 
		// else, walk to the next point
		else {
			agent->walkToNextPoint();
		}
	}
}

/// <summary>
/// Returns the player as target position in the collision grid.
/// </summary>
/// <returns></returns>
Ogre::Vector3 EnemyFollowAStarState::getPlayerAsTarget() {
	Ogre::Vector3 playerPos = GameManager::getSingletonPtr()->getPlayer()->getPosition();
	Coordinate player = Coordinate(playerPos.x, playerPos.z);
	player = GameManager::getSingletonPtr()->getLevelManager()->getLevelGenerator()->getCollisionGridPosition(player);
	playerPos.x = player.x;
	playerPos.z = player.z;

	return playerPos;
}