#include "Player.h"
#include "GameManager.h"
#include "PlayerEquipment.h"
#include "PlayerHealthUpgrade.h"
#include "PlayerDamageUpgrade.h"
#include "Debug.h"

const int Player::LOW_HP = 2;
const int Player::HIGH_HP = 4;
const int Player::LOW_DMG = 2;
const int Player::HIGH_DMG = 5;

/// <summary>
/// Creates a new instance of the <see cref="Player"/> class.
/// </summary>
/// <param name="pMyNode">My node.</param>
/// <param name="pMyEntity">My entity.</param>
Player::Player(Ogre::SceneNode* pMyNode, Ogre::Entity* pMyEntity) : Character(pMyNode, pMyEntity)
{
	// override default speeds
	_movespeed = 560;

	equipment = new PlayerEquipment(40.0f, 3.0f);
	_maxHealth = equipment->getHealth();
	_damage = equipment->getDamage();

	pMyNode->setScale(0.4f, 0.4f, 0.4f);
	pMyNode->setPosition(pMyNode->getPosition().x, 27.0f, pMyNode->getPosition().z);
	pMyEntity->setMaterialName("InGame/BlueHouse");

	upgradeEquipment(PlayerUpgradeType(10, Health));
	upgradeEquipment(PlayerUpgradeType(1, Damage));

	// initialize pLevel vars
	_currentLevel = 1;
	_currentXP = 0;
	_xpTillNextLevel = calcXpTillLevel(_currentLevel + 1);

	_noticeDistance = 300;
	_nearbyNPC = nullptr;
	_inDialog = false;

	_inBattle = false;
	_inBattleTime = 0;
	_totalInBattleTime = 10.0f;

	_lightAttackCooldown = 1.2f;

	_inQuest = true;
}

/// <summary>
/// Resets the player, for a new level or replay or dead
/// </summary>
/// <param name="pMyNode">The player node.</param>
/// <param name="pMyEntity">The player entity.</param>
void Player::reset(Ogre::SceneNode* pMyNode, Ogre::Entity* pMyEntity) {
	_myNode = pMyNode;
	_myEntity = pMyEntity;

	setDirVector(Ogre::Vector3(0, 0, 0));
	pMyNode->setScale(0.4f, 0.4f, 0.4f);
	pMyNode->setPosition(pMyNode->getPosition().x, 27.0f, pMyNode->getPosition().z);
	pMyEntity->setMaterialName("InGame/BlueHouse");

	_target = nullptr;
	_inDialog = false;
	_nearbyNPC = nullptr;

	_inBattle = false;
	_inBattleTime = 0;
}

/// <summary>
/// Called if players health is below 0, resets player within current dungeon
/// </summary>
void Player::die() {
	changeInBattle(false);
	GameManager::getSingletonPtr()->getSoundManager()->triggerEndRoom(false);
	GameManager::getSingletonPtr()->getSoundManager()->playerDead();
	GameManager::getSingletonPtr()->goToState(GameState::Died);

	reset(_myNode, _myEntity);

	Ogre::Vector3 startPos = GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getStartPos();
	startPos.y = 27.0f;
	_myNode->setPosition(startPos);

	equipment = equipment->removeUpgrades();
	_maxHealth = equipment->getHealth();
	_damage = equipment->getDamage();
	_currentHealth = _maxHealth;
	GameManager::getSingleton().getUIManager()->adjustHealthBar(_currentHealth, _maxHealth);

	Debug("player: (Health, Damage)", Coordinate(_maxHealth, _damage));
}

void Player::upgradeEquipment(PlayerUpgradeType upgrade) {
	switch(upgrade.modifier) {
	case Health:
		equipment = new PlayerHealthUpgrade(equipment, upgrade.value);
		_maxHealth = equipment->getHealth();
		_currentHealth = equipment->getHealth();
		break;
	case Damage:
		equipment = new PlayerDamageUpgrade(equipment, upgrade.value);
		_damage = equipment->getDamage();
		break;
	default:
		Debug("Wrong upgrade type");
		break;
	}
	int i = 0;
}

/// <summary>
/// Players update loop, updates instance inbetween frames
/// </summary>
/// <param name="pDeltaTime">The delta time, time since last frame.</param>
void Player::update(Ogre::Real pDeltaTime)
{
	// don't do stuff when in dialog
	if (_inDialog) return;

	// set epic music when close to sis
	if (GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getDistToSis(getPosition()) < GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->scalar * 3) {
		GameManager::getSingletonPtr()->getSoundManager()->triggerEndRoom();
		// if player reached the end 
		if (GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getDistToSis(getPosition()) < _noticeDistance) {
			GameManager::getSingletonPtr()->goNextState();
			GameManager::getSingletonPtr()->getSoundManager()->completed();
			GameManager::getSingletonPtr()->getSoundManager()->triggerEndRoom(false);
		}
	}
	else {
		GameManager::getSingletonPtr()->getSoundManager()->triggerEndRoom(false);
	}

	// call basic update
	Character::update(pDeltaTime);

	if(_inBattle) {
		_inBattleTime += pDeltaTime;

		if(_inBattleTime > _totalInBattleTime) {
			changeInBattle(false);
			_inBattleTime = 0;
		}
	}

	// find nearby NPC's and handle HUD text
	for (int i = 0; i < GameManager::getSingletonPtr()->getLevelManager()->getFriendlyNpcs().size(); ++i) {
		if (getPosition().distance(GameManager::getSingletonPtr()->getLevelManager()->getFriendlyNpcs()[i]->getPosition()) < _noticeDistance) {
			setNearbyNPC(dynamic_cast<Npc*>(GameManager::getSingletonPtr()->getLevelManager()->getFriendlyNpcs()[i]));
			break;
		}
		setNearbyNPC(nullptr);
	}
}

/// <summary>
/// Triggers dialog. Either starts, continues or exits the dialog based on current status of the dialog.
/// </summary>
void Player::dialogTriggered() {
	if (_nearbyNPC == nullptr) return;

	if (!_inDialog) {
		GameManager::getSingletonPtr()->getSoundManager()->dialog();
	}
	_inDialog = true;

	if (!_nearbyNPC->talk(getPosition())) {
		_inDialog = false;
		if (_hasItem && _needToGiveItem && _nearbyNPC->relevantForAction()) {
			giveItem(_nearbyNPC);
		}
	}

	return;
}

/// Adjusts the health.
/// </summary>
/// <param name="pAdjust">The adjustment of health.</param>
/// <returns>False if the player runs out of health and dies.</returns>
bool Player::adjustHealth(float pAdjust)
{
	if (!Character::adjustHealth(pAdjust)) { return false; }

	GameManager::getSingleton().getUIManager()->adjustHealthBar(_currentHealth, _maxHealth);
	return true;
}

/// <summary>
/// Basic player attack.
/// </summary>
/// <returns></returns>
bool Player::lightAttack()
{
	if (!Character::lightAttack()) {
		return false;
	}

	std::vector<Character*> targets = GameManager::getSingletonPtr()->getLevelManager()->getHostileNpcs();
	findTarget(targets);

	if (_target == nullptr) {
		return false;
	}
	
	//deal damage 
	changeInBattle(true);

	if (_target->adjustHealth(_damage)) {
		GameManager::getSingletonPtr()->getUIManager()->adjustEnemyHealthBar(_target->getCurrHealth(), _target->getMaxHealth());
	}
	else {
		changeInBattle(false);
	}

	_inBattleTime = 0;
	_canAttack = false;
	_currAttackCooldown = _lightAttackCooldown;

	return true;
}

/// <summary>
/// Calculates the pXP need to reach the next pLevel.
/// </summary>
/// <param name="pLevel">The next pLevel.</param>
/// <returns></returns>
int Player::calcXpTillLevel(int pLevel)
{
	int newXP = 0;

	for (int i = 1; i < pLevel; ++i)
	{
		newXP += Ogre::Math::Floor(i + 300 * Ogre::Math::Pow(2, i / 7.0f));
	}

	newXP = Ogre::Math::Floor(newXP / 4);

	return newXP;
}

/// <summary>
/// Increases the pXP of the player.
/// </summary>
/// <param name="pXP">The pXP gained.</param>
void Player::gainXP(int pXP)
{
	if ((_currentXP += pXP) >= _xpTillNextLevel)
	{
		levelUp();
	}
}

/// <summary>
/// Levels up the player.
/// </summary>
void Player::levelUp()
{
	++_currentLevel;
	_xpTillNextLevel = calcXpTillLevel(_currentLevel + 1);

	// Increase _stats
}

/// <summary>
/// Sets the nearby NPC variable.
/// </summary>
/// <param name="pNewNPC">The new NPC object.</param>
void Player::setNearbyNPC(Npc* pNewNPC) {
	if (_nearbyNPC == pNewNPC) return;

	_nearbyNPC = pNewNPC;

	if (_nearbyNPC != nullptr) {
		GameManager::getSingletonPtr()->getUIManager()->showHUDText("Press 'E' to talk.");
	}
	else {
		GameManager::getSingletonPtr()->getUIManager()->hideHUDText();
	}
}

/// <summary>
/// Changes the players inbattle state
/// </summary>
/// <param name="val">if set to <c>true</c> the player goes in attack mode.</param>
void Player::changeInBattle(bool val) {
	if (_inBattle == val) return;

	_inBattle = val;

	if (_inBattle) {
		GameManager::getSingletonPtr()->getUIManager()->showEnemyHealthBar();
	}
	else {
		GameManager::getSingletonPtr()->getUIManager()->hideEnemyHealthBar();
	}
}

/// <summary>
/// Calculates the angle between the player and a vector.
/// </summary>
/// <param name="other">Position of the other object.</param>
/// <returns></returns>
float Player::angleBetween(Ogre::Vector3 other) {
	Ogre::Vector3 myDir = _myNode->getOrientation() * Ogre::Vector3(0, 0, -1);
	Ogre::Vector3 dirToSis = other - getPosition();
	myDir.normalise();
	dirToSis.normalise();

	float dotproduct = (myDir.x * dirToSis.x) + (myDir.y * dirToSis.y) + (myDir.z * dirToSis.z);
	Ogre::Vector3 cross = myDir.crossProduct(dirToSis);
	float angle = Ogre::Math::ACos(dotproduct).valueDegrees();

	if(cross.y > 0) angle = -angle;

	return angle;
}

/// <summary>
/// Toggles whether the player is doing a quest right now.
/// </summary>
void Player::toggleInQuest() {
	_inQuest = !_inQuest; 
	GameManager::getSingletonPtr()->getUIManager()->setQuestOn(_inQuest);
}

void Player::recieveItem() {
	Character::recieveItem();

	GameManager::getSingletonPtr()->getQuestManager()->getCurrentQuest()->sendMsg(Action::msgPlayerItem);
}
