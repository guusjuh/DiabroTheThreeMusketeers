#include "BasicEnemy.h"
#include "GameManager.h"
#include "Player.h"
#include "StateMachine.h"
#include "EnemyUpgrade.h"
#include "BaseNpc.h"

const int BasicEnemy::LOW_HP = 5;
const int BasicEnemy::HIGH_HP = 10;
const int BasicEnemy::LOW_DMG = 4;
const int BasicEnemy::HIGH_DMG = 8;
const int BasicEnemy::LOW_NDIST = 5;
const int BasicEnemy::HIGH_NDIST = 20;

const Ogre::ColourValue BasicEnemy::COL_HP = Ogre::ColourValue(0, 1, 0);
const Ogre::ColourValue BasicEnemy::COL_DMG = Ogre::ColourValue(1, 0, 0);
const Ogre::ColourValue BasicEnemy::COL_NDIST = Ogre::ColourValue(0, 0, 1);

/// <summary>
/// Creates a new instance of the <see cref="BasicEnemy"/> class.
/// All enemies inherit from this class.
/// </summary>
/// <param name="pMyNode">My node.</param>
/// <param name="pMyEntity">My entity.</param>
BasicEnemy::BasicEnemy(Ogre::SceneNode* pMyNode, Ogre::SceneNode* pMyRotationNode, Ogre::Entity* pMyEntity, City* pMyCity, int level) 
: BaseNpc(pMyNode, pMyRotationNode, pMyEntity, pMyCity)
{
	// set the states of the FSM
	EnemyFollowState* followPlayer = new EnemyFollowState();
	EnemyAttackState* attack = new EnemyAttackState();
	EnemyMoveAroundCenterState* moveAroundCenter = new EnemyMoveAroundCenterState();
	possibleStates["Follow"] = followPlayer;
	possibleStates["Attack"] = attack;
	possibleStates["AroundCenter"] = moveAroundCenter;
	_initialized = false;

	// subscribe @ levelmanager
	id = GameManager::getSingletonPtr()->getLevelManager()->subscribeHostileNPC(this);

	// set upgrades
	healthUpgrades = 0;
	damageUpgrades = 0;
	noticeDistUpgrades = 0;
	equipment = new EnemyEquipment(20.0f, 2.0f, 200.0f);

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
		_originalMaterialName = "InGame/GreenEnemy";
	} else if (myCol == COL_DMG) {
		_originalMaterialName = "InGame/RedEnemy";
	} else if (myCol == COL_NDIST) {
		_originalMaterialName = "InGame/BlueEnemy";
	}
	pMyEntity->setMaterialName(_originalMaterialName);

	name = getNameOptions()[rand() % getNameOptions().size()];
	_relevantForAction = false;

	// set node
	pMyNode->setScale(0.5f, 0.5f, 0.5f);
	pMyNode->setPosition(pMyNode->getPosition().x, 18.0f, pMyNode->getPosition().z);

	_movespeed = 320;
	_rotationspeed = 0.13f;
	_maxHealth = equipment->getHealth();
	_damage = equipment->getDamage();
	_noticeDistance = equipment->getNoticeDist();
	_attackDistance = 180;
	_lightAttackCooldown = 2.3f;
	_totalHitTime = 0.3f;

	_currentHealth = _maxHealth;
}

void BasicEnemy::assignUpgrades(int level) {
	// random amount based on level
	level = GameManager::getSingletonPtr()->getRandomInRange(level - 2, level + 1);
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


std::vector<std::string> BasicEnemy::getNameOptions() {
	std::vector<std::string> _nameOptions;
	_nameOptions.push_back("Damien");
	_nameOptions.push_back("Eldritch");

	return _nameOptions;
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

/// <summary>
/// Updates the enemey for deltatime.
/// </summary>
/// <param name="pDeltatime">The deltatime.</param>
void BasicEnemy::update(Ogre::Real pDeltatime)
{
	if (!_initialized){
		stateMachine = StateMachine<BaseNpc>(this, "AroundCenter", possibleStates);
		_initialized = true;
	}
	stateMachine.update();
	
	if (_playerDetected) {
	}
	BaseNpc::update(pDeltatime);
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
	equipment = equipment->removeUpgrades();
	_maxHealth = equipment->getHealth();
	_damage = equipment->getDamage();
	_currentHealth = _maxHealth;
	
	healthUpgrades = 0;
	damageUpgrades = 0;
	noticeDistUpgrades = 0;

	Character::die();
	
	GameManager::getSingletonPtr()->getLevelManager()->spawnEnemy(_myCity, 5.0f);

	if (_relevantForAction) {
		GameManager::getSingletonPtr()->getQuestManager()->getCurrentQuest()->sendMsg(Action::msgEnemyDead);
	}

	GameManager::getSingletonPtr()->getLevelManager()->detachHostileNPC(id);
}


