#include "IdleState.h"

IdleState::IdleState() 
{
	
}

IdleState::~IdleState()
{
	
}

void IdleState::Enter(BaseNpc* agent){

}

void IdleState::Execute(BaseNpc* agent){
	agent->setDirVector(Ogre::Vector3(0,0,0));
}

void IdleState::Exit(BaseNpc* agent){

}