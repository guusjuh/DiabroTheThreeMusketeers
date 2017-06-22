#ifndef RANGED_ENEMY_H
#define RANGED_ENEMY_H

#include "BaseEnemy.h"

class RangedEnemy : public BaseEnemy {
public:
	RangedEnemy(Ogre::SceneNode*, Ogre::SceneNode*, Ogre::Entity*, City*, int);
	~RangedEnemy() {}

	void update(Ogre::Real) override;
	bool lightAttack() override;
};

#endif