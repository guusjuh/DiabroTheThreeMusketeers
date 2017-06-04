#include "EnemyFollowDirectState.h"
#include "GameManager.h"

EnemyFollowDirectState::EnemyFollowDirectState()
{

}

EnemyFollowDirectState::~EnemyFollowDirectState()
{

}

void EnemyFollowDirectState::Enter(BaseNpc* agent){

}

void EnemyFollowDirectState::Execute(BaseNpc* agent){
	Ogre::Vector3 playerPos = GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->getPosition();

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

void EnemyFollowDirectState::Exit(BaseNpc* agent){
}

void EnemyFollowDirectState::Collide(BaseNpc* agent){
	agent->stateMachine.setState("Relative");
}