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

	void update(Ogre::Real) override;
	void rotatePivot(Ogre::Vector3);

	void calculateAStar(Ogre::Vector3 targetPos);
	void walkToNextPoint();

	//were protected
	StateMachine<BaseNpc> stateMachine;
	std::vector<Coordinate> nextPos;
	Coordinate goalPos;
	City* _myCity;
	bool _playerDetected;
	
protected:
	Ogre::SceneNode* _myRotationNode;

	virtual void detectPlayer(); 

	void walkTo(Ogre::Vector3);

private:
	float _timer;

	void wander(); 	
};
#endif