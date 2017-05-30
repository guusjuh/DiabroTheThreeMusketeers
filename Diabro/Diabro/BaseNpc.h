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

	int getNextPosSize() { return nextPos.size(); }
	Coordinate getGoalPos() { return goalPos; }
	City* getCity() { return _myCity; }
	bool isPlayerDetected() { return _playerDetected; }
	
protected:
	std::vector<Coordinate> nextPos;
	Ogre::SceneNode* _myRotationNode;

	Coordinate goalPos;
	City* _myCity;
	bool _playerDetected;

	virtual void detectPlayer(); 


private:
	float _timer;

	void wander(); 	
};
#endif