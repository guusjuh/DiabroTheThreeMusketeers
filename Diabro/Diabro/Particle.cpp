#include "Particle.h"
#include "GameManager.h"

Particle::Particle(std::string meshName, std::string materialName, Ogre::Vector3 position, 
	Ogre::Vector3 direction, Ogre::Vector3 scale, float speed, float lifetime, float gravity)
: _direction(direction), _speed(speed), _lifetime(lifetime), _gravity(gravity) {
	_node = GameManager::getSingletonPtr()->getSceneManager()->getRootSceneNode()->createChildSceneNode();
	Ogre::Entity* myEntity = GameManager::getSingletonPtr()->getSceneManager()->createEntity(meshName);
	_node->createChildSceneNode()->attachObject(myEntity);
	_node->setPosition(position);
	_node->setScale(scale);
	_node->lookAt(direction, Ogre::Node::TS_LOCAL, Ogre::Vector3::UNIT_X);
	myEntity->setMaterialName(materialName);

	id = GameManager::getSingletonPtr()->getLevelManager()->subscribeParticle(this);
	int i = 0;
}

void Particle::update(Ogre::Real deltatime) {
	// should i be destroyed due to lifetime?
	_lifetime -= deltatime;
	if (_lifetime < 0) {
		destroy();
		return;
	}

	// no? move
	_node->translate(Ogre::Vector3(1, 0, 0) * _speed * deltatime, Ogre::Node::TS_LOCAL);
	_node->translate(Ogre::Vector3(0, -1, 0) * 150.0f * deltatime, Ogre::Node::TS_WORLD);
}

void Particle::destroy() {
	GameManager::getSingletonPtr()->getLevelManager()->detachParticle(id);

	_node->removeAndDestroyAllChildren();
	GameManager::getSingletonPtr()->getSceneManager()->destroySceneNode(_node);
}

void Particle::castParticles(int amount, Ogre::Vector3 position, std::string mesh, std::string material, float minScale, float maxScale,
	float minSpeed, float maxSpeed, float minLifetime, float maxLifetime, float gravity) {
	for (int i = 0; i < amount; ++i) {
		// get a random point on a unit sphere to set the direction
		Ogre::Vector3 randomDirection = Ogre::Vector3(0, 0, 0);
		do {
			float x = (GameManager::getSingletonPtr()->getRandomInRange(0, 201) - 100) / 100.0f;
			float y = (GameManager::getSingletonPtr()->getRandomInRange(0, 201) - 100) / 100.0f;
			float z = (GameManager::getSingletonPtr()->getRandomInRange(0, 201) - 100) / 100.0f;

			randomDirection = Ogre::Vector3(x, y, z);

		} while (randomDirection.length() > 1);

		randomDirection.normalise();

		// note: the min and max values are multiplied by the amount by which the result is divided
		// this is done so that the developer can pass values that seem legit, but the random method can still generate a random number
		// since the random number only works with integers, this had to be done... :(
		float randomScale = GameManager::getSingletonPtr()->getRandomInRange(minScale * 100, maxScale * 100) / 100.0f;
		float randomSpeed = GameManager::getSingletonPtr()->getRandomInRange(minSpeed, maxSpeed);
		float randomLifeTime = GameManager::getSingletonPtr()->getRandomInRange(minLifetime * 10, maxLifetime * 10) / 10.0f;

		Particle* particle = new Particle(mesh, material, position,
			randomDirection, Ogre::Vector3(randomScale, randomScale, randomScale),
			randomSpeed, randomLifeTime, gravity);
	}
}

