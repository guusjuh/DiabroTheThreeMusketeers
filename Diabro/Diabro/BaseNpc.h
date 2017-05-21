#ifndef BASE_NPC_H_
#define BASE_NPC_H_

#include "Character.h"
#include "City.h"

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
	
protected:
	Ogre::SceneNode* _myRotationNode;
	City* _myCity;

	float _noticeDistance;
	bool _playerDetected;

	void walkToNextPoint();

	void detectPlayer(); 
	void walkTo(Ogre::Vector3);

private:
	float _timer;

	void wander(); 	
};
#endif