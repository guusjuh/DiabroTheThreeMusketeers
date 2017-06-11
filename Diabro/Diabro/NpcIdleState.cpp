#include "NpcIdleState.h"
#include "GameManager.h"

NpcIdleState::NpcIdleState()
{

}

NpcIdleState::~NpcIdleState()
{
}

void NpcIdleState::Enter(BaseNpc* agent){
}

void NpcIdleState::Execute(BaseNpc* agent){
	//check if state transition is needed
	if (!agent->isPlayerDetected()) {
		agent->stateMachine.setState("Wander");
		return;
	}
	if (((Npc*)agent)->isKidnapped()){
		agent->stateMachine.setState("Kidnapped");
		return;
	}
	agent->setDirVector(Ogre::Vector3(0, 0, 0));
}

void NpcIdleState::Exit(BaseNpc* agent){

}