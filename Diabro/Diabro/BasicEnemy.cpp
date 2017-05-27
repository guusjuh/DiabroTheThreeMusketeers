#include "BasicEnemy.h"
#include "GameManager.h"
#include "Player.h"
#include "StateMachine.h"

/// <summary>
/// Creates a new instance of the <see cref="BasicEnemy"/> class.
/// All enemies inherit from this class.
/// </summary>
/// <param name="pMyNode">My node.</param>
/// <param name="pMyEntity">My entity.</param>
BasicEnemy::BasicEnemy(Ogre::SceneNode* pMyNode, Ogre::SceneNode* pMyRotationNode, Ogre::Entity* pMyEntity, City* pMyCity) : BaseNpc(pMyNode, pMyRotationNode, pMyEntity, pMyCity)
{
	State<Character> startState = IdleState();
	std::map<std::string, State<Character>> possibleStates;
	possibleStates["Idle"] = startState;
	stateMachine = StateMachine<Character>(this, startState, possibleStates);

	id = GameManager::getSingletonPtr()->getLevelManager()->subscribeHostileNPC(this);
}

/// <summary>
/// Updates the enemey for deltatime.
/// </summary>
/// <param name="pDeltatime">The deltatime.</param>
void BasicEnemy::update(Ogre::Real pDeltatime)
{
	BaseNpc::update(pDeltatime);
	
	if(_playerDetected) {
		walkTo(GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->getPosition());

		if (getPosition().distance(GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->getPosition()) < _attackDistance) {
			lightAttack();
		}
	}
}


/// <summary>
/// Performs a light attack.
/// </summary>
/// <returns></returns>
bool BasicEnemy::lightAttack()
{
	if (!Character::lightAttack()) {
		return false;
	}

	std::vector<Character*> targets;
	targets.push_back(GameManager::getSingletonPtr()->getLevelManager()->getPlayer());
	findTarget(targets);

	if (_target == nullptr) {
		return false;
	}

	//deal damage 
	_target->adjustHealth(_damage);
	
	_canAttack = false;
	_currAttackCooldown = _lightAttackCooldown;
	return true;
}

/// <summary>
/// Called when the enemy dies, e.g. runs out of health.
/// </summary>
void BasicEnemy::die() {
	Character::die();
	
	GameManager::getSingletonPtr()->getLevelManager()->detachHostileNPC(id);
	GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->gainXP(10);
}


