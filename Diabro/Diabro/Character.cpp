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

	_forwardVec = Ogre::Vector3::ZERO;
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
	if (_dirVec.z > 0){
		_dirVec.z = (_dirVec.z / 3);
	}
	Ogre::Vector3 newPos = _myNode->getPosition() + (_myNode->getOrientation() * (_dirVec * getSpeed() * pDeltatime));
	Zone* zone = GameManager::getSingletonPtr()->getLevelManager()->getLevelGenerator()->getZonePointer(0, 0);

	correctRotation(newPos, zone, _radius);

	newPos = _myNode->getPosition() + (_myNode->getOrientation() * (_dirVec * getSpeed() * pDeltatime));

	if (collidesWithGrid(newPos, zone, _radius) && closestDistanceToNpc(newPos) > _radius)
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
	Ogre::Real distance;
	//hostile Npc
	std::vector<Character*> hostileNpcs = GameManager::getSingletonPtr()->getLevelManager()->getHostileNpcs();
	for (size_t i = 0; i < hostileNpcs.size(); i++)
	{
		if (hostileNpcs[i] != this){
			distance = pos.distance(hostileNpcs[i]->getPosition()) - (hostileNpcs[i]->_radius);

			if (distance < DistanceToClosestTarget){
				DistanceToClosestTarget = distance;
			}
		}
	}
	//friendly Npc
	std::vector<Character*> friendlyNpcs = GameManager::getSingletonPtr()->getLevelManager()->getFriendlyNpcs();
	for (size_t i = 0; i < friendlyNpcs.size(); i++)
	{
		if (friendlyNpcs[i] != this){
			distance = pos.distance(friendlyNpcs[i]->getPosition()) - (friendlyNpcs[i]->_radius);

			if (distance < DistanceToClosestTarget){
				DistanceToClosestTarget = distance;
			}
		}
	}
	//player
	Character* player = GameManager::getSingletonPtr()->getPlayer();
	if (player != this) {
		distance = pos.distance(player->getPosition()) - (player->_radius);
		if (distance < DistanceToClosestTarget) {
			DistanceToClosestTarget = distance;
		}
	}
	//sister
	distance = pos.distance(GameManager::getSingletonPtr()->getLevelManager()->getLevelGenerator()->getSisPos()) - (friendlyNpcs[0]->_radius);//uses the radius of the first friendly npc
	if (distance < DistanceToClosestTarget) {
		DistanceToClosestTarget = distance;
	}

	return DistanceToClosestTarget;
}

void Character::correctRotation(Ogre::Vector3 pos, Zone* zone, int range){
	//4 positions each with an offset for checking the collision with the wall
	float cornerRange = cos(45.0f) * range;
	Ogre::Vector3 tempPos = pos;

	//TODO: list
	// 1. special case whiskers must be defined, a bit outside the player himself.
	// 2. these check for collision with the collision grid.
	// 3. if ONE of them collides, check the player direction and steer by with 45 degrees.
	// 4. if any of the normal points collides, let it do it's thing (e.g. step 1-3 should be done before those).
	// 5. 

	Coordinate left = GameManager::getSingletonPtr()->getLevelManager()->getLevelGenerator()->getCollisionGridPosition(Coordinate(pos.x - range, pos.z));
	Coordinate right = GameManager::getSingletonPtr()->getLevelManager()->getLevelGenerator()->getCollisionGridPosition(Coordinate(pos.x + range, pos.z));
	Coordinate behind = GameManager::getSingletonPtr()->getLevelManager()->getLevelGenerator()->getCollisionGridPosition(Coordinate(pos.x, pos.z - range));
	Coordinate front = GameManager::getSingletonPtr()->getLevelManager()->getLevelGenerator()->getCollisionGridPosition(Coordinate(pos.x, pos.z + range));

	// world direction
	Ogre::Vector3 newDirection = _myNode->getOrientation() * _dirVec;

	// left and right
	if (!(zone->getCollisionGrid()[left.x + left.z * zone->getResolution().x * City::gridScalar]) ||
		!(zone->getCollisionGrid()[right.x + right.z * zone->getResolution().x * City::gridScalar])){
		newDirection.x = 0;
	}

	// up and down
	if (!(zone->getCollisionGrid()[behind.x + behind.z * zone->getResolution().x * City::gridScalar]) ||
		!(zone->getCollisionGrid()[front.x + front.z * zone->getResolution().x * City::gridScalar])){
		newDirection.z = 0;
	}

	// back to local
	Ogre::Quaternion tempQ = _myNode->getOrientation();
	Ogre::Quaternion rotation = tempQ.Inverse();// Ogre::Quaternion(Ogre::Degree(roll.valueDegrees()), Ogre::Vector3(0, 1, 0));
		
	_dirVec = rotation * newDirection;
}


bool Character::collidesWithGrid(Ogre::Vector3 pos, Zone* zone, int range){
	//4 positions each with an offset for checking the collision with the wall
	float cornerRange = cos(45.0f) * range;
	Coordinate temp0 = GameManager::getSingletonPtr()->getLevelManager()->getLevelGenerator()->getCollisionGridPosition(Coordinate(pos.x - range, pos.z));
	Coordinate temp1 = GameManager::getSingletonPtr()->getLevelManager()->getLevelGenerator()->getCollisionGridPosition(Coordinate(pos.x + range, pos.z));
	Coordinate temp2 = GameManager::getSingletonPtr()->getLevelManager()->getLevelGenerator()->getCollisionGridPosition(Coordinate(pos.x, pos.z - range));
	Coordinate temp3 = GameManager::getSingletonPtr()->getLevelManager()->getLevelGenerator()->getCollisionGridPosition(Coordinate(pos.x, pos.z + range));
	Coordinate temp4 = GameManager::getSingletonPtr()->getLevelManager()->getLevelGenerator()->getCollisionGridPosition(Coordinate(pos.x - cornerRange, pos.z - cornerRange));
	Coordinate temp5 = GameManager::getSingletonPtr()->getLevelManager()->getLevelGenerator()->getCollisionGridPosition(Coordinate(pos.x + cornerRange, pos.z - cornerRange));
	Coordinate temp6 = GameManager::getSingletonPtr()->getLevelManager()->getLevelGenerator()->getCollisionGridPosition(Coordinate(pos.x - cornerRange, pos.z + cornerRange));
	Coordinate temp7 = GameManager::getSingletonPtr()->getLevelManager()->getLevelGenerator()->getCollisionGridPosition(Coordinate(pos.x + cornerRange, pos.z + cornerRange));

	return(zone->getCollisionGrid()[temp0.x + temp0.z * zone->getResolution().x * City::gridScalar] &&
		zone->getCollisionGrid()[temp1.x + temp1.z * zone->getResolution().x * City::gridScalar] &&
		zone->getCollisionGrid()[temp2.x + temp2.z * zone->getResolution().x * City::gridScalar] &&
		zone->getCollisionGrid()[temp3.x + temp3.z * zone->getResolution().x * City::gridScalar] &&
		zone->getCollisionGrid()[temp4.x + temp4.z * zone->getResolution().x * City::gridScalar] &&
		zone->getCollisionGrid()[temp5.x + temp5.z * zone->getResolution().x * City::gridScalar] &&
		zone->getCollisionGrid()[temp6.x + temp6.z * zone->getResolution().x * City::gridScalar] &&
		zone->getCollisionGrid()[temp7.x + temp7.z * zone->getResolution().x * City::gridScalar]);
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

void Character::die() {
	int amount = GameManager::getSingletonPtr()->getRandomInRange(12, 16);

	Particle::castParticles(amount, getPosition(), "uv_sphere.mesh", "InGame/ParticleDead",
		0.05f, 0.07f, 300, 450, 0.3f, 0.4f, 260.0f);
}

/// <summary>
/// Character dies.
/// </summary>
void Character::destroy()
{
	_myNode->removeAndDestroyAllChildren();
	GameManager::getSingletonPtr()->getSceneManager()->destroySceneNode(_myNode);
}

void Character::recieveItem() {
	Debug("\tCharacter: I received an item!");
		
	_hasItem = true;
}

/// <summary>
/// Calculates the angle between the player and a vector.
/// </summary>
/// <param name="other">Position of the other object.</param>
/// <returns></returns>
float Character::angleBetween(Ogre::Vector3 other) {
	if (_myNode == nullptr) return 0;

	Ogre::Vector3 myDir = _myNode->getOrientation() * _forwardVec;
	Ogre::Vector3 dirToSis = other - getPosition();
	myDir.normalise();
	dirToSis.normalise();

	float dotproduct = (myDir.x * dirToSis.x) + (myDir.y * dirToSis.y) + (myDir.z * dirToSis.z);
	Ogre::Vector3 cross = myDir.crossProduct(dirToSis);
	float angle = Ogre::Math::ACos(dotproduct).valueDegrees();

	if (cross.y > 0) angle = -angle;

	return angle;
}