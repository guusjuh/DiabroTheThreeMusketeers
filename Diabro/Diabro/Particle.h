#ifndef PARTICLE_H_
#define PARTICLE_H_
#include <OgreSceneNode.h>

class Particle {
public:
	Particle();
	~Particle();

	void update();

private:
	Ogre::SceneNode* _node;
	Ogre::Vector3 _direction;
	float _speed;
	float _lifetime;

	void destroy();
};

#endif