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
	possibleStates["FollowAStar"] = new EnemyFollowAStarState();
	possibleStates["FollowDirect"] = new EnemyFollowDirectState();
	possibleStates["Attack"] = new EnemyAttackState();
	possibleStates["Relative"] = new EnemyWalkToPointNearPlayerState();
	possibleStates["AroundCenter"] = new EnemyMoveAroundCenterState();
	possibleStates["Charge"] = new EnemyChargeState();
	_initialized = false;

	// subscribe @ levelmanager
	id = GameManager::getSingletonPtr()->getLevelManager()->subscribeHostileNPC(this);

	// set upgrades
	healthUpgrades = 0;
	damageUpgrades = 0;
	noticeDistUpgrades = 0;
	equipment = new EnemyEquipment(15.0f, 1.5f, Zone::scalar * 1.5f);

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

	int colorIndex = 0;
	if(damageUpgrades == 0 && healthUpgrades == 0 && noticeDistUpgrades == 0) {
		colorIndex = 0;
	} else if(damageUpgrades > healthUpgrades) {
		if(damageUpgrades > noticeDistUpgrades) {
			colorIndex = 2;
		} else {
			colorIndex = 3;
		}
	} else if (noticeDistUpgrades > healthUpgrades){
		colorIndex = 3;
	} else {
		colorIndex = 1;
	}

	_rotationType = GameManager::getSingletonPtr()->getRandomInRange(0, 2);

	//set color
	if(colorIndex == 0) {
		if (_rotationType == 0){
			_originalMaterialName = "InGame/GreyStripeEnemy";
		}
		else if (_rotationType == 1){
			_originalMaterialName = "InGame/GreyDotEnemy";
		}
	}
	else if (colorIndex == 1) {
		if (_rotationType == 0){
			_originalMaterialName = "InGame/GreenStripeEnemy";
		}
		else if (_rotationType == 1){
			_originalMaterialName = "InGame/GreenDotEnemy";
		}
	}
	else if (colorIndex == 2) {
		if (_rotationType == 0){
			_originalMaterialName = "InGame/RedStripeEnemy";
		}
		else if (_rotationType == 1){
			_originalMaterialName = "InGame/RedDotEnemy";
		}
	}
	else if (colorIndex == 3) {
		if (_rotationType == 0){
			_originalMaterialName = "InGame/BlueStripeEnemy";
		}
		else if (_rotationType == 1){
			_originalMaterialName = "InGame/BlueDotEnemy";
		}
	}
	pMyEntity->setMaterialName(_originalMaterialName);

	name = "squared bastard";
	_relevantForAction = false;

	// set node
	pMyNode->setScale(0.5f, 0.5f, 0.5f);
	pMyNode->setPosition(pMyNode->getPosition().x, 18.0f, pMyNode->getPosition().z);
	indicatorNode->setScale(0.15f, 0.15f, 0.15f);
	indicatorNode->setPosition(0.0f, 100.0f, 0.0f);

	_movespeed = 320;
	_rotationspeed = 180.0f;
	_maxHealth = equipment->getHealth();
	_damage = equipment->getDamage();
	_noticeDistance = equipment->getNoticeDist();
	_attackDistance = 110;
	_lightAttackCooldown = 1.2f;
	_totalHitTime = 0.3f;
	_radius = 25.0f;

	_currentHealth = _maxHealth;

	_isDead = false;
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
	timePast = pDeltatime;

	Ogre::Vector3 scale = Ogre::Vector3(0.5f, 0.5f, 0.5f) * (1.5f - std::abs(_currAttackCooldown / _lightAttackCooldown - 0.5f));
	_myNode->setScale(scale);

	if (!_initialized){
		stateMachine = StateMachine<BaseNpc>(this, "AroundCenter", possibleStates);
		_initialized = true;
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
	_isDead = true;

	// display hud text if this enemy is important for quests
	if (_hasItem && _needToGiveItem) {
		giveItem(GameManager::getSingletonPtr()->getLevelManager()->getPlayer());
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

	Character::die();
	
	static float slowIncrease = 0;
	slowIncrease += 0.01f;
	GameManager::getSingletonPtr()->getLevelManager()->spawnEnemy(_myCity, false);

	if (_relevantForAction) {
		GameManager::getSingletonPtr()->getQuestManager()->getCurrentQuest()->sendMsg(this, Action::msgEnemyDead);
	}

	GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->adjustHealth(-(GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->getMaxHealth() / 100) * 2);
	GameManager::getSingletonPtr()->getLevelManager()->detachHostileNPC(id);
}


