#include "Character.h"
#include "GameManager.h"
#include "Debug.h"
#include <math.h>

/// <summary>
/// Initializes a new instance of the <see cref="Character"/> class.
/// This class is contained by the <see cref="LevelManager"/>, which manages all instances in the level. 
/// All characters in the game, NPC's and the player, inherit from this base class. 
/// </summary>
/// <param name="pMyNode">My node.</param>
/// <param name="pMyEntity">My entity.</param>
Character::Character(Ogre::SceneNode* pMyNode, Ogre::Entity* pMyEntity) : _myNode(pMyNode), _myEntity(pMyEntity),  _dirVec(0, 0, 0),
_movespeed(100), _rotationspeed(0.13), _currentLevel(1), _currentHealth(0), _canAttack(true), _maxHealth(40),
_attackDistance(100), _currAttackCooldown(0), _lightAttackCooldown(5.0f), _hitted(false), _totalHitTime(.5f), _damage(3), _noticeDistance(400.0f)
{
	_currentHealth = _maxHealth;
	isPlayer = false;
}

/// <summary>
/// Updates the frame based on the specified pDeltatime.
/// </summary>
/// <param name="pDeltatime">The time since last frame.</param>
void Character::update(Ogre::Real pDeltatime)
{
	if (_currAttackCooldown > 0) {
		_currAttackCooldown -= pDeltatime;
	}else {
		_canAttack = true;
	}

	if (_hitTime > 0) {
		_hitTime -= pDeltatime;
		return;
	}
	else {
		_hitted = false;
	}

	_dirVec.normalise();
	Ogre::Vector3 newPos = _myNode->getPosition() + (_myNode->getOrientation() * (_dirVec * getSpeed() * pDeltatime));
	float range = 40;
	float cornerRange = cos(45.0f) * range ;
	//4 positions each with an offset for checking the collision with the wall
	Coordinate temp0 = GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getGridPosition(Coordinate(newPos.x - range, newPos.z));
	Coordinate temp1 = GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getGridPosition(Coordinate(newPos.x + range, newPos.z));
	Coordinate temp2 = GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getGridPosition(Coordinate(newPos.x, newPos.z - range));
	Coordinate temp3 = GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getGridPosition(Coordinate(newPos.x, newPos.z + range));
	Coordinate temp4 = GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getGridPosition(Coordinate(newPos.x - cornerRange, newPos.z - cornerRange));
	Coordinate temp5 = GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getGridPosition(Coordinate(newPos.x + cornerRange, newPos.z - cornerRange));
	Coordinate temp6 = GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getGridPosition(Coordinate(newPos.x - cornerRange, newPos.z + cornerRange));
	Coordinate temp7 = GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getGridPosition(Coordinate(newPos.x + cornerRange, newPos.z + cornerRange));


	Ogre::Real DistanceToClosestTarget = 1000;
	std::vector<Character*> hostileNpcs = GameManager::getSingletonPtr()->getLevelManager()->getHostileNpcs();
	for (size_t i = 0; i < hostileNpcs.size(); i++)
	{
		if (hostileNpcs[i] != this){
			Ogre::Real distance = newPos.distance(hostileNpcs[i]->getPosition());

			if (distance < DistanceToClosestTarget){
				DistanceToClosestTarget = distance;
			}
		}
	}
	std::vector<Character*> friendlyNpcs = GameManager::getSingletonPtr()->getLevelManager()->getFriendlyNpcs();
	for (size_t i = 0; i < friendlyNpcs.size(); i++)
	{
		if (friendlyNpcs[i] != this){
			Ogre::Real distance = newPos.distance(friendlyNpcs[i]->getPosition());

			if (distance < DistanceToClosestTarget){
				DistanceToClosestTarget = distance;
			}
		}
	}
	
	Zone* zone = GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getZonePointer(0, 0);
	if (zone->getCollisionGrid()[temp0.x + temp0.z * zone->_width] && 
		zone->getCollisionGrid()[temp1.x + temp1.z * zone->_width] && 
		zone->getCollisionGrid()[temp2.x + temp2.z * zone->_width] &&
		zone->getCollisionGrid()[temp3.x + temp3.z * zone->_width] &&
		zone->getCollisionGrid()[temp4.x + temp4.z * zone->_width] &&
		zone->getCollisionGrid()[temp5.x + temp5.z * zone->_width] &&
		zone->getCollisionGrid()[temp6.x + temp6.z * zone->_width] &&
		zone->getCollisionGrid()[temp7.x + temp7.z * zone->_width] &&
		DistanceToClosestTarget > 50)
	{
		_myNode->translate(_dirVec * getSpeed() * pDeltatime, Ogre::Node::TS_LOCAL);
	}
	else if(!isPlayer){
		_myNode->translate(-10 * _dirVec * getSpeed() * pDeltatime, Ogre::Node::TS_LOCAL);
	}
}

//TODO: these methods should be generic
/// <summary>
/// Finds the target.
/// </summary>
/// <param name="pPossibleTargets">The possible targets vector.</param>
void Character::findTarget(std::vector<Character*> pPossibleTargets)
{
	for (int i = 0; i < pPossibleTargets.size(); ++i) {
		if (getPosition().distance(pPossibleTargets[i]->getPosition()) < _attackDistance) {
			//TODO: check for allignment with the other character
			_target = pPossibleTargets[i];
			return;
		}
	}

	_target = nullptr;
}

/// <summary>
/// Set the direction vector of the character with the specified vector.
/// </summary>
/// <param name="pMoveVec">The new direction vector.</param>
void Character::move(Ogre::Vector3& pMoveVec)
{
	_dirVec = pMoveVec;
}

/// <summary>
/// Performs a light attack.
/// </summary>
/// <returns></returns>
bool Character::lightAttack()
{
	if (!_canAttack || _hitted)
	{
		return false;
	}

	return true;
}
/// <summary>
/// Adjusts the health.
/// </summary>
/// <param name="pAdjust">The adjustment of health.</param>
/// <returns>False if the character runs out of health.</returns>
bool Character::adjustHealth(float pAdjust)
{
	GameManager::getSingletonPtr()->getSoundManager()->hit();

	_hitTime = _totalHitTime;
	_hitted = true;

	if ((_currentHealth -= pAdjust) <= 0)
	{
		die();
		return false;
	}
	Debug("I got hit...", _currentHealth);
	return true;
}

/// <summary>
/// Character dies.
/// </summary>
void Character::die()
{
	_myNode->setVisible(false);

	//TODO: clean up the memory.. 

	//TODO: actually destroy the node and its children
	//_myNode->removeAndDestroyAllChildren();
	//GameManager::getSingletonPtr()->getSceneManager()->destroySceneNode(_myNode);
}