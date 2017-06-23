#include "BaseEnemy.h"
#include "GameManager.h"
#include "Player.h"
#include "StateMachine.h"
#include "EnemyUpgrade.h"
#include "BaseNpc.h"

const int BaseEnemy::LOW_HP = 2;
const int BaseEnemy::HIGH_HP = 7;
const int BaseEnemy::LOW_DMG = 4;
const int BaseEnemy::HIGH_DMG = 8;
const int BaseEnemy::LOW_NDIST = 5;
const int BaseEnemy::HIGH_NDIST = 20;

const Ogre::ColourValue BaseEnemy::COL_HP = Ogre::ColourValue(0, 1, 0);
const Ogre::ColourValue BaseEnemy::COL_DMG = Ogre::ColourValue(1, 0, 0);
const Ogre::ColourValue BaseEnemy::COL_NDIST = Ogre::ColourValue(0, 0, 1);

/// <summary>
/// Creates a new instance of the <see cref="BaseEnemy"/> class.
/// All enemies inherit from this class.
/// </summary>
/// <param name="pMyNode">My node.</param>
/// <param name="pMyEntity">My entity.</param>
BaseEnemy::BaseEnemy(Ogre::SceneNode* pMyNode, Ogre::SceneNode* pMyRotationNode, Ogre::Entity* pMyEntity, City* pMyCity, int level) 
: BaseNpc(pMyNode, pMyRotationNode, pMyEntity, pMyCity)
{
	_initialized = false;

	// subscribe @ levelmanager
	id = GameManager::getSingletonPtr()->getLevelManager()->subscribeHostileNPC(this);

	// set upgrades
	healthUpgrades = 0;
	damageUpgrades = 0;
	noticeDistUpgrades = 0;
	equipment = new EnemyEquipment(15.0f, 1.5f, Zone::scalar * 1.5f);

	assignUpgrades(level);

	name = "squared bastard";
	_relevantForAction = false;

	// set node
	pMyNode->setScale(0.5f, 0.5f, 0.5f);
	pMyNode->setPosition(pMyNode->getPosition().x, 18.0f, pMyNode->getPosition().z);
	indicatorNode->setScale(0.15f, 0.15f, 0.15f);
	indicatorNode->setPosition(0.0f, 100.0f, 0.0f);

	_maxHealth = equipment->getHealth();
	_damage = equipment->getDamage();
	_noticeDistance = equipment->getNoticeDist();

	_movespeed = 0;
	_rotationspeed = 0;
	_attackDistance = 0;
	_lightAttackCooldown = 0;
	_totalHitTime = 0.3f;
	_radius = 25.0f;

	_currentHealth = _maxHealth;
}

UpgradeModifierType BaseEnemy::getMostUsedUpgrade() {
	IEnemyEquipment* tempEquipment = equipment;
	while (tempEquipment != nullptr) {
		if (tempEquipment->isBase()) {
			tempEquipment = nullptr;
			continue;
		}

		switch (tempEquipment->getType()) {
		case Health:
			healthUpgrades++;
			break;
		case Damage:
			damageUpgrades++;
			break;
		case NoticeDist:
			noticeDistUpgrades++;
			break;
		default:
			break;
		}

		tempEquipment = tempEquipment->getBase();
	}
	// remove standard added amount of heath upgrades
	healthUpgrades = healthUpgrades - GameManager::getSingletonPtr()->getLevelManager()->getCurrentLevel();

	UpgradeModifierType mostUsedUpgrade = None;
	if (damageUpgrades == 0 && healthUpgrades == 0 && noticeDistUpgrades == 0) {
		mostUsedUpgrade = None;
	}
	else if (damageUpgrades > healthUpgrades) {
		if (damageUpgrades > noticeDistUpgrades) {
			mostUsedUpgrade = Damage;
		}
		else {
			mostUsedUpgrade = NoticeDist;
		}
	}
	else if (noticeDistUpgrades > healthUpgrades) {
		mostUsedUpgrade = NoticeDist;
	}
	else {
		mostUsedUpgrade = Health;
	}

	return mostUsedUpgrade;
}

void BaseEnemy::assignUpgrades(int level) {
	// assign healthupgrades based on level 
	for (int i = 0; i < level; i++) {
		// random value between low and high
		int value = GameManager::getSingletonPtr()->getRandomInRange(LOW_HP, HIGH_HP);

		upgradeEquipment(EnemyUpgradeType(value, Health));
	}
	// random amount based on level
	level = GameManager::getSingletonPtr()->getRandomInRange(level - 2, level + 2);
	if (level < 0) level = 0;

	// set variables used for creation of each upgrade.
	UpgradeModifierType type = (UpgradeModifierType)0;
	int low = 0;
	int high = 1;
	int value = low;

	// for each upgrade to be created.
	for (int i = 0; i < level; ++i) {
		// random type
		type = (UpgradeModifierType)GameManager::getSingletonPtr()->getRandomInRange(1, AMOUNT_OF_UPGRADE_TYPES);

		switch (type) {
		case Health:
			low = LOW_HP;
			high = HIGH_HP;
			break;
		case Damage:
			low = LOW_DMG;
			high = HIGH_DMG;
			break;
		case NoticeDist:
			low = LOW_NDIST;
			high = HIGH_NDIST;
			break;
		default:
			break;
		}

		// random value between low and high
		value = GameManager::getSingletonPtr()->getRandomInRange(low, high);

		upgradeEquipment(EnemyUpgradeType(value, type));
	}
}

void BaseEnemy::upgradeEquipment(EnemyUpgradeType upgrade) {
	switch (upgrade.modifier) {
	case Health:
		equipment = new EnemyHealthUpgrade(equipment, upgrade.value, Health);
		_maxHealth = equipment->getHealth();
		_currentHealth = equipment->getHealth();
		break;
	case Damage:
		equipment = new EnemyDamageUpgrade(equipment, upgrade.value, Damage);
		_damage = equipment->getDamage();
		break;
	case NoticeDist:
		equipment = new EnemyNoticeDistUpgrade(equipment, upgrade.value, NoticeDist);
		_noticeDistance = equipment->getNoticeDist();
		break;
	default:
		Debug("Wrong upgrade type.");
		break;
	}
}

/// <summary>
/// Updates the enemey for deltatime.
/// </summary>
/// <param name="pDeltatime">The deltatime.</param>
void BaseEnemy::update(Ogre::Real pDeltatime)
{
	if (!_initialized){
		stateMachine = StateMachine<BaseNpc>(this, "AroundCenter", possibleStates);
		_initialized = true;
	}
	
	BaseNpc::update(pDeltatime);
}

void BaseEnemy::collide() {
	std::string state = stateMachine.getCurrentState();

	if(state == "FollowAStar" || state == "AroundCenter"  || state == "FollowDirect")
	{
		walkToNeighbour();
	}
}

/// <summary>
/// Performs a light attack.
/// </summary>
/// <returns></returns>
bool BaseEnemy::lightAttack()
{
	if (!Character::lightAttack()) {
		return false;
	}

	std::vector<Character*> targets;
	targets.push_back(GameManager::getSingletonPtr()->getPlayer());
	findTarget(targets);

	if (_target == nullptr) {
		return false;
	}

	_canAttack = false;
	_currAttackCooldown = _lightAttackCooldown;

	return true;
}

/// <summary>
/// Called when the enemy dies, e.g. runs out of health.
/// </summary>
void BaseEnemy::die() {
	_isDead = true;

	// display hud text if this enemy is important for quests
	if (_hasItem && _needToGiveItem) {
		giveItem(GameManager::getSingletonPtr()->getPlayer());
	}
	if (_relevantForAction) {
		GameManager::getSingletonPtr()->getUIManager()->showHUDText(GameManager::getSingletonPtr()->getQuestManager()->obtainDialog(this), 3.5f);
		GameManager::getSingletonPtr()->getQuestManager()->getCurrentQuest()->sendMsg(this, Action::msgPlayerInfo);
	}

	equipment = equipment->removeUpgrades();
	_maxHealth = equipment->getHealth();
	_damage = equipment->getDamage();
	_currentHealth = _maxHealth;
	
	healthUpgrades = 0;
	damageUpgrades = 0;
	noticeDistUpgrades = 0;
	
	static float slowIncrease = 0;
	slowIncrease += 0.01f;
	GameManager::getSingletonPtr()->getLevelManager()->spawnEnemy(_myCity, false);

	if (_relevantForAction) {
		GameManager::getSingletonPtr()->getQuestManager()->getCurrentQuest()->sendMsg(this, Action::msgEnemyDead);
	} 

	GameManager::getSingletonPtr()->getPlayer()->adjustHealth(-(GameManager::getSingletonPtr()->getPlayer()->getMaxHealth() / 100) * 2);
	GameManager::getSingletonPtr()->getLevelManager()->detachHostileNPC(id);

/*	// note: we can sneaky still check for relevant, since it's relevant till the next update
	if(!_relevantForAction) {
		Character::destroy();
	}*/

	Character::die();
	Character::destroy();
}


