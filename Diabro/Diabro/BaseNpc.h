#ifndef BASE_NPC_H_
#define BASE_NPC_H_

#include "Character.h"
#include "City.h"

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
	std::vector<Coordinate> nextPos;
	Coordinate goalPos;

	City* getMyCity() { return _myCity; }

	using IQuestContent::getType;

protected:
	Ogre::SceneNode* _myRotationNode;
	City* _myCity;
	bool _playerDetected;

	//TODO: implement a pointer so the npc can keep track of the current quest
	//Quest* _currentQuest;

	//TODO: move the dialog variables/methods from NPC to here and change a few things so enemies can handle this too.
	// 1. make sure enemy dialog can still end (since they actually died)
	// 2. switch between

	virtual void detectPlayer(); 
	void walkToNextPoint();

	void walkTo(Ogre::Vector3);

private:
	float _timer;

	void wander(); 	
};
#endif