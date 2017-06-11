#include "NpcKidnappedState.h"
#include "GameManager.h"

NpcKidnappedState::NpcKidnappedState()
{

}

NpcKidnappedState::~NpcKidnappedState()
{
}

void NpcKidnappedState::Enter(BaseNpc* agent){
}

void NpcKidnappedState::Execute(BaseNpc* agent){
	//check if state transition is needed
	if (agent->getPosition().distance(GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->getPosition()) < agent->getNoticeDistance()) {
		agent->stateMachine.setState("FollowPlayer");
		return;
	}
	agent->setDirVector(Ogre::Vector3(0, 0, 0));
}

void NpcKidnappedState::Exit(BaseNpc* agent){

}