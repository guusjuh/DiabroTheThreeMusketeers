#include "NpcWanderState.h"
#include "GameManager.h"

NpcWanderState::NpcWanderState()
{

}

NpcWanderState::~NpcWanderState()
{
}

void NpcWanderState::Enter(BaseNpc* agent){
}

void NpcWanderState::Execute(BaseNpc* agent){
	if (agent->isPlayerDetected()) {
		agent->stateMachine.setState("Idle");
		return;
	}
	if (((Npc*)agent)->isKidnapped()){
		agent->stateMachine.setState("Kidnapped");
		return;
	}

	if (agent->getDirVector() == Ogre::Vector3::ZERO) agent->setDirVector(Ogre::Vector3(1, 0, 0));

	if (agent->getPosition().distance(Ogre::Vector3(agent->getGoalPos().x, agent->getPosition().y, agent->getGoalPos().z)) < 50) {
		if (agent->getNextPosSize() == 0) {
			RealCoordinate _randomTownPos = agent->getCity()->getNpcPosition();
			Ogre::Vector3 pos = Ogre::Vector3(_randomTownPos.rx, agent->getPosition().y, _randomTownPos.rz);
			Ogre::Vector3 goalPosition = Ogre::Vector3(agent->getGoalPos().x / agent->getCity()->Scalar(), agent->getPosition().y, agent->getGoalPos().z / agent->getCity()->Scalar());
			while (goalPosition.distance(pos) < 1) {
				_randomTownPos = agent->getCity()->getNpcPosition();
				pos = Ogre::Vector3(_randomTownPos.rx, agent->getPosition().y, _randomTownPos.rz);
			}
			pos.x = pos.x / Zone::scalar * City::gridScalar;
			pos.z = pos.z / Zone::scalar * City::gridScalar;

			agent->calculateAStar(pos);
		}
		else {
			agent->walkToNextPoint();
		}
	}
}

void NpcWanderState::Exit(BaseNpc* agent){
}