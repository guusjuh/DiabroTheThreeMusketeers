#ifndef PARTICLE_H_
#define PARTICLE_H_
#include <OgreSceneNode.h>

class Particle {
public:
	Particle(std::string meshName, std::string materialName, Ogre::Vector3 position, Ogre::Vector3 direction, Ogre::Vector3 scale, float speed, float lifetime);
	~Particle() {}

	int id;

	void update(Ogre::Real deltatime);

private:
	Ogre::SceneNode* _node;
	Ogre::Vector3 _direction;
	float _speed;
	float _lifetime;

	void destroy();
};

#endif