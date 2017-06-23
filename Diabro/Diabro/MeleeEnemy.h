#ifndef MELEE_ENEMY_H
#define MELEE_ENEMY_H

#include "BaseEnemy.h"

class MeleeEnemy : public BaseEnemy {
public:
	MeleeEnemy(Ogre::SceneNode*, Ogre::SceneNode*, Ogre::Entity*, City*, int);
	~MeleeEnemy() {}

	void update(Ogre::Real) override;
	bool lightAttack() override;
};

#endif