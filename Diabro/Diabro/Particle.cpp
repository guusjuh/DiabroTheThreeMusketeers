#include "Particle.h"
#include "GameManager.h"

Particle::Particle(std::string meshName, std::string materialName, Ogre::Vector3 position, 
	Ogre::Vector3 direction, Ogre::Vector3 scale, float speed, float lifetime)
: _direction(direction), _speed(speed), _lifetime(lifetime) {
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
	_node->translate(Ogre::Vector3(1, 0.1f, 0) * _speed * deltatime, Ogre::Node::TS_LOCAL);
}

void Particle::destroy() {
	GameManager::getSingletonPtr()->getLevelManager()->detachParticle(id);

	_node->removeAndDestroyAllChildren();
	GameManager::getSingletonPtr()->getSceneManager()->destroySceneNode(_node);
}


