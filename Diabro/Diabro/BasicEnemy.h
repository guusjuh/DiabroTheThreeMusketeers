#ifndef BASE_ENEMY_H_
#define BASE_ENEMY_H_

#include "BaseNpc.h"
#include "StateMachine.h"
#include "IdleState.h"

class BasicEnemy : public BaseNpc
{
public:
	BasicEnemy(Ogre::SceneNode*, Ogre::SceneNode*, Ogre::Entity*, City*);
	~BasicEnemy() {}

	void update(Ogre::Real) override;
	void die() override;


	StateMachine<Character> stateMachine;
private:
	bool lightAttack() override;
};

#endif
