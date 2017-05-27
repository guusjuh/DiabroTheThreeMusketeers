#include "BasicEnemy.h"
#include "GameManager.h"
#include "Player.h"
#include "StateMachine.h"
#include "EnemyUpgrade.h"

const int BasicEnemy::LOW_HP = 2;
const int BasicEnemy::HIGH_HP = 5;
const int BasicEnemy::LOW_DMG = 2;
const int BasicEnemy::HIGH_DMG = 7;
const int BasicEnemy::LOW_NDIST = 10;
const int BasicEnemy::HIGH_NDIST = 30;

const Ogre::ColourValue BasicEnemy::COL_HP = Ogre::ColourValue(0, 1, 0);
const Ogre::ColourValue BasicEnemy::COL_DMG = Ogre::ColourValue(1, 0, 0);
const Ogre::ColourValue BasicEnemy::COL_NDIST = Ogre::ColourValue(0, 0, 1);

/// <summary>
/// Creates a new instance of the <see cref="BasicEnemy"/> class.
/// All enemies inherit from this class.
/// </summary>
/// <param name="pMyNode">My node.</param>
/// <param name="pMyEntity">My entity.</param>
BasicEnemy::BasicEnemy(Ogre::SceneNode* pMyNode, Ogre::SceneNode* pMyRotationNode, Ogre::Entity* pMyEntity, City* pMyCity, int level) : BaseNpc(pMyNode, pMyRotationNode, pMyEntity, pMyCity)
{
	State<Character> startState = IdleState();
	std::map<std::string, State<Character>> possibleStates;
	possibleStates["Idle"] = startState;
	stateMachine = StateMachine<Character>(this, startState, possibleStates);

	id = GameManager::getSingletonPtr()->getLevelManager()->subscribeHostileNPC(this);

	healthUpgrades = 0;
	damageUpgrades = 0;
	noticeDistUpgrades = 0;

	equipment = new EnemyEquipment(20.0f, 2.0f, 200.0f);
	_maxHealth = equipment->getHealth();
	_damage = equipment->getDamage();
	_noticeDistance = equipment->getNoticeDist();

	assignUpgrades(level);

	IEnemyEquipment* tempEquipment = equipment;
	while(tempEquipment != nullptr) {
		if(tempEquipment->isBase()) {
			tempEquipment = nullptr;
			continue;
		}

		switch(tempEquipment->getType()) {
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

	delete tempEquipment;

	Ogre::ColourValue myCol = COL_HP;
	if(damageUpgrades > healthUpgrades) {
		if(damageUpgrades > noticeDistUpgrades) {
			myCol = COL_DMG;
		} else {
			myCol = COL_NDIST;
		}
	} else if (noticeDistUpgrades > healthUpgrades){
		myCol = COL_NDIST;
	}

	//set color
	if(myCol == COL_HP) {
		pMyEntity->setMaterial(Ogre::MaterialManager::getSingletonPtr()->getByName("Houses/Green"));
	} else if (myCol == COL_DMG) {
		pMyEntity->setMaterial(Ogre::MaterialManager::getSingletonPtr()->getByName("Houses/Red"));
	} else if (myCol == COL_NDIST) {
		pMyEntity->setMaterial(Ogre::MaterialManager::getSingletonPtr()->getByName("Houses/Blue"));
	}

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
	printf("Level: %d \n", level);
	printf("H: %f D: %f ND: %f \n", _maxHealth, _damage, _noticeDistance);
	printf("H nr: %d D nr: %d ND nr: %d \n", healthUpgrades, damageUpgrades, noticeDistUpgrades);
	fclose(fp);
#endif
}

void BasicEnemy::assignUpgrades(int level) {
	// random amount based on level
	level = GameManager::getSingletonPtr()->getRandomInRange(level - 1, level + 2);
	if (level < 1) level = 1;

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

void BasicEnemy::upgradeEquipment(EnemyUpgradeType upgrade) {
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
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		FILE* fp;
		freopen_s(&fp, "CONOUT$", "w", stdout);
		printf("wrong upgrade type\n");
		fclose(fp);
#endif
		break;
	}
}

void BasicEnemy::update(Ogre::Real pDeltatime)
{
	BaseNpc::update(pDeltatime);
	
	/*if(_playerDetected) {
		walkTo(GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->getPosition());

		if (getPosition().distance(GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->getPosition()) < _attackDistance) {
			lightAttack();
		}
	}*/
}

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

void BasicEnemy::die() {
	equipment = equipment->removeUpgrades();
	_maxHealth = equipment->getHealth();
	_damage = equipment->getDamage();
	_currentHealth = _maxHealth;
	
	healthUpgrades = 0;
	damageUpgrades = 0;
	noticeDistUpgrades = 0;

	Character::die();
	
	GameManager::getSingletonPtr()->getLevelManager()->detachHostileNPC(id);
	GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->gainXP(10);
}


