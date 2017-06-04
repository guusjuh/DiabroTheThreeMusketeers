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
_movespeed(0), _rotationspeed(0), _currentLevel(1), _currentHealth(0), _canAttack(true), _maxHealth(1), _hasItem(false), _needToGiveItem(false), _radius(1),
_attackDistance(0), _currAttackCooldown(0), _lightAttackCooldown(0), _hitted(false), _totalHitTime(0), _damage(0), _noticeDistance(0), _totalFlashTimeOnHit(0.1f)
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
		timeSinceHit += pDeltatime;
		if (timeSinceHit > _totalFlashTimeOnHit) {
			_myEntity->setMaterialName(_originalMaterialName);
			gotHitTimerActive = false;
		}
		return;
	}
	else {
		_hitted = false;
	}

	_dirVec.normalise();
	Ogre::Vector3 newPos = _myNode->getPosition() + (_myNode->getOrientation() * (_dirVec * getSpeed() * pDeltatime));
	
	Zone* zone = GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getZonePointer(0, 0);
	if (collidesWithGrid(newPos, zone, _radius) && closestDistanceToNpc(newPos) > 50)
	{
		_myNode->translate(_dirVec * getSpeed() * pDeltatime, Ogre::Node::TS_LOCAL);
	}
	else{
		collide();
	}
}

void Character::collide(){

}

Ogre::Real Character::closestDistanceToNpc(Ogre::Vector3 pos){
	Ogre::Real DistanceToClosestTarget = 1000;
	std::vector<Character*> hostileNpcs = GameManager::getSingletonPtr()->getLevelManager()->getHostileNpcs();
	for (size_t i = 0; i < hostileNpcs.size(); i++)
	{
		if (hostileNpcs[i] != this){
			Ogre::Real distance = pos.distance(hostileNpcs[i]->getPosition());

			if (distance < DistanceToClosestTarget){
				DistanceToClosestTarget = distance;
			}
		}
	}
	std::vector<Character*> friendlyNpcs = GameManager::getSingletonPtr()->getLevelManager()->getFriendlyNpcs();
	for (size_t i = 0; i < friendlyNpcs.size(); i++)
	{
		if (friendlyNpcs[i] != this){
			Ogre::Real distance = pos.distance(friendlyNpcs[i]->getPosition());

			if (distance < DistanceToClosestTarget){
				DistanceToClosestTarget = distance;
			}
		}
	}
	return DistanceToClosestTarget;
}

bool Character::collidesWithGrid(Ogre::Vector3 pos, Zone* zone, int range){
	//4 positions each with an offset for checking the collision with the wall
	float cornerRange = cos(45.0f) * range;
	Coordinate temp0 = GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getCollisionGridPosition(Coordinate(pos.x - range, pos.z));
	Coordinate temp1 = GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getCollisionGridPosition(Coordinate(pos.x + range, pos.z));
	Coordinate temp2 = GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getCollisionGridPosition(Coordinate(pos.x, pos.z - range));
	Coordinate temp3 = GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getCollisionGridPosition(Coordinate(pos.x, pos.z + range));
	Coordinate temp4 = GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getCollisionGridPosition(Coordinate(pos.x - cornerRange, pos.z - cornerRange));
	Coordinate temp5 = GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getCollisionGridPosition(Coordinate(pos.x + cornerRange, pos.z - cornerRange));
	Coordinate temp6 = GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getCollisionGridPosition(Coordinate(pos.x - cornerRange, pos.z + cornerRange));
	Coordinate temp7 = GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getCollisionGridPosition(Coordinate(pos.x + cornerRange, pos.z + cornerRange));

	return(zone->getCollisionGrid()[temp0.x + temp0.z * zone->_width * City::gridScalar] &&
		zone->getCollisionGrid()[temp1.x + temp1.z * zone->_width * City::gridScalar] &&
		zone->getCollisionGrid()[temp2.x + temp2.z * zone->_width * City::gridScalar] &&
		zone->getCollisionGrid()[temp3.x + temp3.z * zone->_width * City::gridScalar] &&
		zone->getCollisionGrid()[temp4.x + temp4.z * zone->_width * City::gridScalar] &&
		zone->getCollisionGrid()[temp5.x + temp5.z * zone->_width * City::gridScalar] &&
		zone->getCollisionGrid()[temp6.x + temp6.z * zone->_width * City::gridScalar] &&
		zone->getCollisionGrid()[temp7.x + temp7.z * zone->_width * City::gridScalar]);
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
	_hitTime = _totalHitTime;
	_hitted = true;
	Debug("I got hit...", _currentHealth);
	if (pAdjust > 0){
		GameManager::getSingletonPtr()->getSoundManager()->hit();
		_myEntity->setMaterialName("InGame/Hit");
		Debug("I think i changed my material");
		gotHitTimerActive = true;
		timeSinceHit = 0;
	}

	if ((_currentHealth -= pAdjust) <= 0)
	{
		die();
		return false;
	}
	else if(_currentHealth > _maxHealth) {
		_currentHealth = _maxHealth;
	}

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

void Character::recieveItem() {
	Debug("\tCharacter: I received an item!");
		
	_hasItem = true;
}
