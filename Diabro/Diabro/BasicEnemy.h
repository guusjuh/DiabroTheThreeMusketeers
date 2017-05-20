#ifndef BASE_ENEMY_H_
#define BASE_ENEMY_H_

#include "BaseNpc.h"

class BasicEnemy : public BaseNpc
{
public:
	BasicEnemy(Ogre::SceneNode*, Ogre::SceneNode*, Ogre::Entity*, City*);
	~BasicEnemy() {}

	void update(Ogre::Real) override;
	void die() override;

private:
	bool lightAttack() override;
};

#endif
