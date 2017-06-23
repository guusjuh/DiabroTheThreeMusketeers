#ifndef PARTICLE_H_
#define PARTICLE_H_
#include <OgreSceneNode.h>

class Particle {
public:
	Particle(std::string meshName, std::string materialName, Ogre::Vector3 position, Ogre::Vector3 direction, Ogre::Vector3 scale, float speed, float lifetime, float gravity);
	~Particle() {}

	int id;

	void update(Ogre::Real deltatime);

	static void castParticles(int amount, Ogre::Vector3 position, std::string mesh, std::string material, 
		float minScale, float maxScale,	float minSpeed, float maxSpeed, float minLifetime, float maxLifetime, float gravity);

private:
	Ogre::SceneNode* _node;
	Ogre::Vector3 _direction;
	float _speed;
	float _lifetime;
	float _gravity;

	void destroy();
};

#endif