#include "WanderState.h"

WanderState::WanderState()
{

}

WanderState::~WanderState()
{

}

void WanderState::Enter(BaseNpc* agent){

}

void WanderState::Execute(BaseNpc* agent){    //for initialization
	//TODO: use -1 -1 as values
	if (agent->goalPos.x == 0 && agent->goalPos.z == 0){
		agent->calculateAStar(agent->_myCity->getRandomPointInRoom());
	}

	//_myNode->translate(_dirVec * getSpeed() * pDeltatime, Ogre::Node::TS_LOCAL);
	if (!agent->_playerDetected) {
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

}

void WanderState::Exit(BaseNpc* agent){

}