#ifndef BULLET_H_
#define BULLET_H_
#include <OgreEntity.h>
#include <OgreRoot.h>
#include "RangedEnemy.h"

class Bullet {
public:
	Bullet(Ogre::SceneNode* node, float dmg, RangedEnemy* enemy);
	~Bullet(){}

	int id;

	void update(Ogre::Real deltaTime);

private:
	float _speed;
	float _lifetime;
	float _radius;
	float _damage;

	int _particleAmount;

	Ogre::SceneNode* _node;
	RangedEnemy* _enemy;

	void impact();
	void destroy();
	void instantiateParticle();
};

#endif