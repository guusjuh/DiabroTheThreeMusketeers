#include "RangedEnemy.h"
#include "EnemyRangedAttackState.h"
#include "EnemyIdleState.h"
#include "GameManager.h"
#include "Bullet.h"

RangedEnemy::RangedEnemy(Ogre::SceneNode* pMyNode, Ogre::SceneNode* pMyRotationNode, Ogre::Entity* pMyEntity, City* pMyCity, int pLevel)
	: BaseEnemy(pMyNode, pMyRotationNode, pMyEntity, pMyCity, pLevel)
{
	// set the states of the FSM
	possibleStates["Attack"] = new EnemyRangedAttackState();
	possibleStates["AroundCenter"] = new EnemyMoveAroundCenterState("Idle");
	possibleStates["Idle"] = new EnemyIdleState();

	// set upgrades
	equipment = new EnemyEquipment(7.0f, 1.5f, Zone::scalar * 1.5f);
	assignUpgrades(pLevel);
	_maxHealth = equipment->getHealth();
	_damage = equipment->getDamage();
	_noticeDistance = equipment->getNoticeDist();

	_movespeed = GameManager::getSingletonPtr()->getRandomInRange(200, 300);
	_rotationspeed = 200.0f;
	_attackDistance = _noticeDistance;
	_lightAttackCooldown = 2.8f;

	_currentHealth = _maxHealth;

	// set material according to upgrades.
	UpgradeModifierType mostUsedUpgrade = getMostUsedUpgrade();

	if (mostUsedUpgrade == 0) { _originalMaterialName = "InGame/GreyDotEnemy"; }
	else if (mostUsedUpgrade == 1) { _originalMaterialName = "InGame/GreenDotEnemy"; }
	else if (mostUsedUpgrade == 2) { _originalMaterialName = "InGame/RedDotEnemy"; }
	else if (mostUsedUpgrade == 3) { _originalMaterialName = "InGame/BlueDotEnemy"; }

	pMyEntity->setMaterialName(_originalMaterialName);
}

bool RangedEnemy::lightAttack() {
	if (!BaseEnemy::lightAttack()) return false;

	Ogre::SceneNode* bulletNode = GameManager::getSingletonPtr()->getSceneManager()->getRootSceneNode()->createChildSceneNode();
	Ogre::Entity* bulletEntity = GameManager::getSingletonPtr()->getSceneManager()->createEntity("uv_sphere.mesh");
	bulletNode->createChildSceneNode()->attachObject(bulletEntity);
	bulletNode->setPosition(getPosition());
	bulletNode->setScale(0.1f, 0.1f, 0.1f);
	bulletEntity->setMaterialName("InGame/Bullet");
	Bullet* bulletScript = new Bullet(bulletNode, equipment->getDamage(), this);

	return true;
}

void RangedEnemy::update(Ogre::Real deltaTime) {
	Ogre::Vector3 scale = Ogre::Vector3(0.5f, 0.5f, 0.5f) * (0.5f + std::abs(_currAttackCooldown / _lightAttackCooldown - 0.5f));
	_myNode->setScale(scale);

	BaseEnemy::update(deltaTime);
}



