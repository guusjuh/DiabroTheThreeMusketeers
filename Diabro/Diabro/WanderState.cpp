#include "WanderState.h"
#include "GameManager.h"

WanderState::WanderState()
{

}

WanderState::~WanderState()
{

}

void WanderState::Enter(BaseNpc* agent){
	agent->calculateAStar(agent->getCity()->getRandomPointInRoom());
}

void WanderState::Execute(BaseNpc* agent){    

	if (agent->getPosition().distance(GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->getPosition()) < agent->getNoticeDistance()){
		agent->stateMachine.setState("Follow");
	}

	if (agent->getPosition().distance(Ogre::Vector3(agent->getGoalPos().x, agent->getPosition().y, agent->getGoalPos().z)) < 50){
		if (agent->getNextPosSize() == 0){
			Ogre::Vector3 pos = Ogre::Vector3(agent->getCity()->getRandomPointInRoom());
			Ogre::Vector3 goalPosition = Ogre::Vector3(agent->getGoalPos().x / agent->getCity()->Scalar(), agent->getPosition().y, agent->getGoalPos().z / agent->getCity()->Scalar());
			while (goalPosition.distance(pos) < 1) {
				pos = Ogre::Vector3(agent->getCity()->getRandomPointInRoom());
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