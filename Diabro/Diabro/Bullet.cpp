#include "Bullet.h"
#include "GameManager.h"

Bullet::Bullet(Ogre::SceneNode* node, float dmg, RangedEnemy* enemy) {
	_enemy = enemy;
	id = GameManager::getSingletonPtr()->getLevelManager()->subscribeBullet(this);
	
	_speed = 900.0f;
	_damage = dmg;
	_lifetime = 2.0f;
	_radius = 10.0f;
	_particleAmount = GameManager::getSingletonPtr()->getRandomInRange(8, 12);

	_node = node;
	_node->lookAt(Ogre::Vector3(GameManager::getSingletonPtr()->getPlayer()->getPosition().x,
		_node->getPosition().y, GameManager::getSingletonPtr()->getPlayer()->getPosition().z),
		Ogre::Node::TS_WORLD, Ogre::Vector3::UNIT_X);
}

void Bullet::update(Ogre::Real deltaTime) {
	// should i be destroyed due to lifetime?
	_lifetime -= deltaTime;
	if (_lifetime < 0) {
		destroy();
		return;
	}

	// should i be destroyed due to collision?
	float distance = 100000.0f;

	// 1. player
	distance = _node->getPosition().distance(GameManager::getSingletonPtr()->getPlayer()->getPosition());
	if(distance < (_radius + GameManager::getSingletonPtr()->getPlayer()->getRadius())) {
		impact();
		return;
	}

	// 2. sis
	distance = _node->getPosition().distance(GameManager::getSingletonPtr()->getLevelManager()->getLevelGenerator()->getSisPos());
	if (distance < (_radius + GameManager::getSingletonPtr()->getLevelManager()->getFriendlyNpcs()[0]->getRadius())) {
		destroy();
		return;
	}

	// 3. collision grid
	Coordinate myCoord = GameManager::getSingletonPtr()->getLevelManager()->getLevelGenerator()->getCollisionGridPosition(Coordinate(_node->getPosition().x, _node->getPosition().z));
	if (!(GameManager::getSingletonPtr()->getLevelManager()->getLevelGenerator()->getZone(0, 0).getCollisionGrid()[myCoord.x + myCoord.z * GameManager::getSingletonPtr()->getLevelManager()->getLevelGenerator()->getZone(0, 0).getResolution().x * City::gridScalar])) {
		destroy();
		return;
	}

	// 4. enemies
	std::vector<Character*> hostileNpcs = GameManager::getSingletonPtr()->getLevelManager()->getHostileNpcs();
	for (size_t i = 0; i < hostileNpcs.size(); i++)
	{
		if(hostileNpcs[i] != _enemy) {
			distance = _node->getPosition().distance(hostileNpcs[i]->getPosition()) - (hostileNpcs[i]->getRadius());

			if (distance < (_radius + hostileNpcs[i]->getRadius())) {
				destroy();
				return;
			}
		}
	}

	// 5. npc's
	std::vector<Character*> friendlyNpcs = GameManager::getSingletonPtr()->getLevelManager()->getFriendlyNpcs();
	for (size_t i = 0; i < friendlyNpcs.size(); i++)
	{
		distance = _node->getPosition().distance(friendlyNpcs[i]->getPosition()) - (friendlyNpcs[i]->getRadius());

		if (distance < (_radius + friendlyNpcs[i]->getRadius())) {
			destroy();
			return;
		}
	}

	_node->translate(Ogre::Vector3(1, 0, 0) * _speed * deltaTime, Ogre::Node::TS_LOCAL);
}

void Bullet::impact() {
	GameManager::getSingletonPtr()->getPlayer()->adjustHealth(_damage);
	Particle::castParticles(_particleAmount, _node->getPosition(), "uv_sphere.mesh", "InGame/ParticleBullet",
		0.02f, 0.05f, 400, 550, 0.3f, 0.4f, 150.0f);

	destroy();
}

void Bullet::destroy() {

	GameManager::getSingletonPtr()->getLevelManager()->detachBullet(id);

	_node->removeAndDestroyAllChildren();
	GameManager::getSingletonPtr()->getSceneManager()->destroySceneNode(_node);
}



