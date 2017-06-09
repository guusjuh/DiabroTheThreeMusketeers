#ifndef BASE_NPC_H_
#define BASE_NPC_H_

#include "Character.h"
#include "City.h"
#include "StateMachine.h"

/// <summary>
/// The super class for all NPC's in the game
/// </summary>
/// <seealso cref="Character" />
/// <seealso cref="IQuestContent" />
class BaseNpc : public Character, public IQuestContent
{
public:
	BaseNpc(Ogre::SceneNode*, Ogre::SceneNode*, Ogre::Entity*, City* );
	BaseNpc() {}
	~BaseNpc() {}

	void collide() override;
	void update(Ogre::Real) override;
	void rotatePivot(Ogre::Vector3);

	void calculateAStar(Ogre::Vector3 targetPos);
	void walkToNextPoint();
	void walkTo(Ogre::Vector3);

	StateMachine<BaseNpc> stateMachine;
	std::map<std::string, State<BaseNpc>*> possibleStates;

	int getNextPosSize() { return nextPos.size(); }
	Coordinate getGoalPos() { return goalPos; }
	City* getCity() { return _myCity; }
	bool isPlayerDetected() { return _playerDetected; }

	City* getMyCity() { return _myCity; }

	using IQuestContent::getType;

	Ogre::Vector3 getQuestPosition() override { return getPosition(); }

	using Character::lightAttack;

	void walkToNeighbour();

protected:
	std::vector<Coordinate> nextPos;
	bool _inCurrentQuest;

	Ogre::SceneNode* _myRotationNode;

	Coordinate goalPos;
	City* _myCity;
	bool _playerDetected;

	//TODO: implement a pointer so the npc can keep track of the current quest
	//Quest* _currentQuest;

	//TODO: move the dialog variables/methods from NPC to here and change a few things so enemies can handle this too.
	// 1. make sure enemy dialog can still end (since they actually died)
	// 2. switch between

	virtual void detectPlayer(); 

	void setRelevantForAction(bool val) override {
		// don't change if u don't have to
		if (_relevantForAction == val) return;

		IQuestContent::setRelevantForAction(val);

		// if you're relevant, set the indicator on with correct color
		if(_relevantForAction) {
			questIndicatorEntity->setMaterialName(_activeMaterial);
			indicatorNode->setVisible(true);
		} else {
			indicatorNode->setVisible(false);
		}
	}

	static const Ogre::String _questMaterial;
	static const Ogre::String _activeMaterial;
	Ogre::Entity* questIndicatorEntity;
	Ogre::SceneNode* indicatorNode;

private:
	float _timer;

	float _degreePerFrame;

	float _wantedRotationAngle;

	void wander();
};
#endif