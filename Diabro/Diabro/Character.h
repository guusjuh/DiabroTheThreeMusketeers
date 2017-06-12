#ifndef CHARACTER_H_
#define CHARACTER_H_

#pragma once
#include <OgrePrerequisites.h>
#include "OgreEntity.h"
#include "BaseApplication.h"
#include "Zone.h"
#include "Debug.h"

/// <summary>
/// The Character class is the superclass for all different kinds of characters.
/// It contains methods and variables for movement and combat.
/// Player and BaseNPC both inherit from this class. 
/// </summary>
class Character
{
	friend class Action;
public:
	Character(Ogre::SceneNode*, Ogre::Entity*);
	Character() : _hasItem(false), _needToGiveItem(false) {}
	~Character() {}

	int id; //may only be changed by levelmanager, death scenario 

	virtual void collide();
	virtual void update(Ogre::Real);

	virtual void move(Ogre::Vector3&);
	virtual bool adjustHealth(float);
	virtual void die();

	Ogre::Vector3 getPosition() {
		if(_myNode != nullptr) return _myNode->getPosition();
		else return Ogre::Vector3(0, 0, 0);
	}
	virtual Ogre::Real getSpeed() { return _movespeed; }
	Ogre::Real getRotationspeed() { return _rotationspeed; }
	Ogre::Vector3 getDirVector() { return _dirVec; }
	Ogre::Vector3 setDirVector(Ogre::Vector3 pMoveVec) { return _dirVec = pMoveVec; }

	Ogre::Real getCurrHealth() { return _currentHealth;  }
	Ogre::Real getMaxHealth() { return _maxHealth;  }
	int getLevel() { return _currentLevel; }
	virtual bool lightAttack();
	Ogre::Real getAttackDistance() { return _attackDistance; }
	float getNoticeDistance() { return _noticeDistance; }
	bool isPlayer;

	Ogre::Real closestDistanceToNpc(Ogre::Vector3 pos);
	bool collidesWithGrid(Ogre::Vector3, Zone* zone, int cornerRange);
	void correctRotation(Ogre::Vector3, Zone* zone, int cornerRange);

	virtual void recieveItem();
	void needToGiveItem() {
		_needToGiveItem = true;
	}

	bool hasItem() { return _hasItem; }

	float angleBetween(Ogre::Vector3);

	float getRadius() { return _radius; }

protected:
	Ogre::Real _movespeed;
	Ogre::Real _rotationspeed;
	Ogre::Vector3 _dirVec;
	Ogre::Vector3 _forwardVec;

	int _currentLevel;

	Ogre::Real _currentHealth;
	Ogre::Real _maxHealth;
	
	Ogre::Real _totalHitTime;
	Ogre::Real _totalFlashTimeOnHit;
	Ogre::Real _hitTime;
	bool _hitted;

	bool _canAttack;
	Ogre::Real _damage;
	Ogre::Real _attackDistance;
	Ogre::Real _currAttackCooldown;
	Ogre::Real _lightAttackCooldown;
	float _noticeDistance;

	Character* _target;
	virtual void findTarget(std::vector<Character*>);

	bool _hasItem;
	bool _needToGiveItem;

	virtual void giveItem(Character* reciever) {
		_hasItem = false;
		_needToGiveItem = false;
		reciever->recieveItem();
	}

	Ogre::SceneNode* _myNode;
	Ogre::Entity* _myEntity;

	std::string _originalMaterialName;
	float timeSinceHit;
	bool gotHitTimerActive;

	float _radius;

	void resetItem() {
		_hasItem = false;
		_needToGiveItem = false;
	}

private:
};

#endif
