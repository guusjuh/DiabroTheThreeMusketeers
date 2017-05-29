#include "WanderState.h"
#include "GameManager.h"

WanderState::WanderState()
{

}

WanderState::~WanderState()
{

}

void WanderState::Enter(BaseNpc* agent){
	agent->calculateAStar(agent->_myCity->getRandomPointInRoom());
}

void WanderState::Execute(BaseNpc* agent){    

	if (agent->getPosition().distance(GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->getPosition()) < agent->getNoticeDistance()){
		agent->stateMachine.setState("Follow");
	}

	if (agent->getPosition().distance(Ogre::Vector3(agent->goalPos.x, agent->getPosition().y, agent->goalPos.z)) < 50){
		if (agent->nextPos.size() == 0){
			Ogre::Vector3 pos = Ogre::Vector3(agent->_myCity->getRandomPointInRoom());
			Ogre::Vector3 goalPosition = Ogre::Vector3(agent->goalPos.x / agent->_myCity->scalar, agent->getPosition().y, agent->goalPos.z / agent->_myCity->scalar);
			while (goalPosition.distance(pos) < 1) {
				pos = Ogre::Vector3(agent->_myCity->getRandomPointInRoom());
			}
			agent->calculateAStar(pos);
		}
		else{
			agent->walkToNextPoint();
		}
	}
}

void WanderState::Exit(BaseNpc* agent){

}