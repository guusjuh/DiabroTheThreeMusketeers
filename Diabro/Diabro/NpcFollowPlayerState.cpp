#include "NpcFollowPlayerState.h"
#include "GameManager.h"

NpcFollowPlayerState::NpcFollowPlayerState()
{

}

NpcFollowPlayerState::~NpcFollowPlayerState()
{
}

void NpcFollowPlayerState::Enter(BaseNpc* agent){
	Ogre::Vector3 playerPos = GameManager::getSingletonPtr()->getPlayer()->getPosition();
	Coordinate player = Coordinate(playerPos.x, playerPos.z);
	player = GameManager::getSingletonPtr()->getLevelManager()->getLevelGenerator()->getCollisionGridPosition(player);
	playerPos.x = player.x;
	playerPos.z = player.z;
	agent->calculateAStar(playerPos);
	agent->walkToNextPoint();
}

void NpcFollowPlayerState::Execute(BaseNpc* agent){
	//check if state transition is needed
	if (agent->getPosition().distance(GameManager::getSingletonPtr()->getPlayer()->getPosition()) > agent->getNoticeDistance()) {
		agent->stateMachine.setState("Kidnapped");
		return;
	}
	if (agent->getCity()->inThisCity(agent->getPosition())){
		((Npc*)agent)->setKidnapped(false);
		agent->stateMachine.setState("Idle");
		return;
	}

	if (agent->getPosition().distance(Ogre::Vector3(agent->getGoalPos().x, agent->getPosition().y, agent->getGoalPos().z)) < agent->getSpeed() / 2.0f || agent->getNextPosSize() == 0){
		int scale = Zone::scalar;
		Ogre::Vector3 playerPos = GameManager::getSingletonPtr()->getPlayer()->getPosition();
		Coordinate player = Coordinate(playerPos.x, playerPos.z);
		player = GameManager::getSingletonPtr()->getLevelManager()->getLevelGenerator()->getCollisionGridPosition(player);
		playerPos.x = player.x;
		playerPos.z = player.z;
		agent->calculateAStar(playerPos);

		if (agent->getNextPosSize() > 0){
			agent->walkToNextPoint();
		}
	}
}

void NpcFollowPlayerState::Exit(BaseNpc* agent){

}