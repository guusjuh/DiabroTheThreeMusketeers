#ifndef CHARACTER_H_
#define CHARACTER_H_

#pragma once
#include <OgrePrerequisites.h>
#include "OgreEntity.h"
#include "BaseApplication.h"

/// <summary>
/// The Character class is the superclass for all different kinds of characters.
/// It contains methods and variables for movement and combat.
/// Player and BaseNPC both inherit from this class. 
/// </summary>
class Character
{
public:
	Character(Ogre::SceneNode*, Ogre::Entity*);
	Character() {}
	~Character() {}

	int id; //may only be changed by levelmanager, death scenario 

	virtual void update(Ogre::Real);
	virtual void move(Ogre::Vector3&);
	virtual bool adjustHealth(float);
	virtual void die();

	Ogre::Vector3 getPosition() { return _myNode->getPosition(); }
	Ogre::Real getSpeed() { return _movespeed; }
	Ogre::Real getRotationspeed() { return _rotationspeed; }
	Ogre::Vector3 getDirVector() { return _dirVec; }
	Ogre::Vector3 setDirVector(Ogre::Vector3 pMoveVec) { return _dirVec = pMoveVec; }

	Ogre::Real getCurrHealth() { return _currentHealth;  }
	int getLevel() { return _currentLevel; }

protected:
	Ogre::Real _movespeed;
	Ogre::Real _rotationspeed;
	Ogre::Vector3 _dirVec;

	int _currentLevel;

	Ogre::Real _currentHealth;
	Ogre::Real _maxHealth;
	
	Ogre::Real _totalHitTime;
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
	virtual bool lightAttack();

	Ogre::SceneNode* _myNode;
	Ogre::Entity* _myEntity;

private:
};

#endif
