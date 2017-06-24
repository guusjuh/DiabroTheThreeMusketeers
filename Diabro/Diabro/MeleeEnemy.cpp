#include "MeleeEnemy.h"
#include "GameManager.h"

MeleeEnemy::MeleeEnemy(Ogre::SceneNode* pMyNode, Ogre::SceneNode* pMyRotationNode, Ogre::Entity* pMyEntity, City* pMyCity, int pLevel)
	: BaseEnemy(pMyNode, pMyRotationNode, pMyEntity, pMyCity, pLevel)
{
	// set the states of the FSM
	possibleStates["FollowAStar"] = new EnemyFollowAStarState();
	possibleStates["FollowDirect"] = new EnemyFollowDirectState();
	possibleStates["Attack"] = new EnemyMeleeAttackState();
	possibleStates["Relative"] = new EnemyWalkToPointNearPlayerState();
	possibleStates["AroundCenter"] = new EnemyMoveAroundCenterState("FollowAStar");

	// set upgrades
	equipment = new EnemyEquipment(15.0f, 1.5f, Zone::scalar * 1.5f);
	assignUpgrades(pLevel);
	_maxHealth = equipment->getHealth();
	_damage = equipment->getDamage();
	_noticeDistance = equipment->getNoticeDist();
	
	_movespeed = GameManager::getSingletonPtr()->getRandomInRange(250, 450);
	_rotationspeed = 180.0f;
	_attackDistance = 110;
	_lightAttackCooldown = 1.2f;

	_currentHealth = _maxHealth;

	// set material according to upgrades.
	UpgradeModifierType mostUsedUpgrade = getMostUsedUpgrade();

	if (mostUsedUpgrade == 0) { _originalMaterialName = "InGame/GreyStripeEnemy"; }
	else if (mostUsedUpgrade == 1) { _originalMaterialName = "InGame/GreenStripeEnemy"; }
	else if (mostUsedUpgrade == 2) { _originalMaterialName = "InGame/RedStripeEnemy"; }
	else if (mostUsedUpgrade == 3) { _originalMaterialName = "InGame/BlueStripeEnemy"; }

	pMyEntity->setMaterialName(_originalMaterialName);
}

void MeleeEnemy::update(Ogre::Real deltaTime) {
	Ogre::Vector3 scale = Ogre::Vector3(0.5f, 0.5f, 0.5f) * (1.5f - std::abs(_currAttackCooldown / _lightAttackCooldown - 0.5f));
	_myNode->setScale(scale);

	BaseEnemy::update(deltaTime);
}

bool MeleeEnemy::lightAttack() {
	if (!BaseEnemy::lightAttack()) {
		return false;
	}

	//deal damage 
	_target->adjustHealth(_damage);

	return true;
}
