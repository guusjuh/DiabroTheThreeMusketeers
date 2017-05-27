#include "Character.h"
#include "GameManager.h"
#include "Debug.h"

/// <summary>
/// Initializes a new instance of the <see cref="Character"/> class.
/// This class is contained by the <see cref="LevelManager"/>, which manages all instances in the level. 
/// All characters in the game, NPC's and the player, inherit from this base class. 
/// </summary>
/// <param name="pMyNode">My node.</param>
/// <param name="pMyEntity">My entity.</param>
Character::Character(Ogre::SceneNode* pMyNode, Ogre::Entity* pMyEntity) : _myNode(pMyNode), _myEntity(pMyEntity),  _dirVec(0, 0, 0),
_movespeed(100), _rotationspeed(0.13), _currentLevel(1), _currentHealth(0), _canAttack(true), _maxHealth(40),
_attackDistance(250), _currAttackCooldown(0), _lightAttackCooldown(5.0f), _hitted(false), _totalHitTime(.5f), _damage(3), _noticeDistance(400.0f)
{
	_currentHealth = _maxHealth;

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
	Coordinate temp = GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getGridPosition(Coordinate(newPos.x, newPos.z));


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
	if (zone->getCollisionGrid()[temp.x + temp.z * zone->_width] && DistanceToClosestTarget > 50)
	{
		_myNode->translate(_dirVec * getSpeed() * pDeltatime, Ogre::Node::TS_LOCAL);
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