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
	// set equipment
	equipment = new PlayerEquipment(30.0f, 5.0f);
	_maxHealth = equipment->getHealth();
	_damage = equipment->getDamage();
	_healthUpgrades = 1;
	_damageUpgrades = 1;
	curCityId = 1;

	// set node vars
	pMyNode->setScale(0.4f, 0.4f, 0.4f);
	pMyNode->setPosition(pMyNode->getPosition().x, 27.0f, pMyNode->getPosition().z);
	_originalMaterialName = "InGame/Player";
	pMyEntity->setMaterialName(_originalMaterialName);
	_forwardVec = Ogre::Vector3(0, 0, -1);

	_nearbyNPC = nullptr;
	_inDialog = false;
	_sisNearby = false;

	_inBattle = false;
	_inBattleTime = 0;
	_totalInBattleTime = 5.0f;

	_inQuest = true;
	isPlayer = true;

	_movespeed = 550;
	_runspeed = 800;
	_isRunning = false;

	_rotationspeed = 0.13f;
	_maxHealth = equipment->getHealth();
	_damage = equipment->getDamage();
	_noticeDistance = 100;
	_attackDistance = 170;
	_lightAttackCooldown = 0.5f;
	_totalHitTime = 0.11f;
	_radius = 30.0f;

	_currentHealth = _maxHealth;

	
	Ogre::Entity* attackSphere = GameManager::getSingletonPtr()->getSceneManager()->createEntity("uv_sphere.mesh");
	attackNode = pMyNode->createChildSceneNode();
	attackNode->attachObject(attackSphere);
	attackNode->setPosition(Ogre::Vector3(getAttackDistance() * .75f, 0, 0));
	attackNode->setScale(0.25f, 0.25f, 0.25f);
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

	changeInBattle(false);
	_target = nullptr;
	_inDialog = false;
	_nearbyNPC = nullptr;
	_sisNearby = false;

	_inBattle = false;
	_inBattleTime = 0;
	curCityId = 1;

	GameManager::getSingleton().getUIManager()->adjustHealthBar(_currentHealth, _maxHealth);

	Ogre::Entity* attackSphere = GameManager::getSingletonPtr()->getSceneManager()->createEntity("uv_sphere.mesh");
	attackNode = pMyNode->createChildSceneNode();
	attackNode->attachObject(attackSphere);
	attackNode->setPosition(Ogre::Vector3(getAttackDistance() * .75f, 0, 0));
	attackNode->setScale(0.25f, 0.25f, 0.25f);
}

/// <summary>
/// Called if players health is below 0, resets player within current dungeon
/// </summary>
void Player::die() {
	_myEntity->setMaterialName(_originalMaterialName);
	GameManager::getSingleton().getUIManager()->adjustHealthBar(0, _maxHealth);

	changeInBattle(false);
	GameManager::getSingletonPtr()->getSoundManager()->triggerEndRoom(false);
	GameManager::getSingletonPtr()->getSoundManager()->playerDead();
	GameManager::getSingletonPtr()->goToState(GameState::Died);

	equipment = equipment->removeUpgrades();
	_maxHealth = equipment->getHealth();
	_damage = equipment->getDamage();
	_healthUpgrades = 1;
	_damageUpgrades = 1;

	_currentHealth = _maxHealth;

	_myNode = nullptr;
	_myEntity = nullptr;

	GameManager::getSingleton().getUIManager()->resetUpgradeText();
	GameManager::getSingletonPtr()->getUIManager()->resetFloorText();
}

void Player::upgradeEquipment(PlayerUpgradeType upgrade) {
	switch(upgrade.modifier) {
	case Health:
		equipment = new PlayerHealthUpgrade(equipment, upgrade.value);
		_maxHealth = equipment->getHealth();
		_healthUpgrades++;
		GameManager::getSingletonPtr()->getUIManager()->setUpgradeText(Health);
		break;
	case Damage:
		equipment = new PlayerDamageUpgrade(equipment, upgrade.value);
		_damage = equipment->getDamage();
		_damageUpgrades++;
		GameManager::getSingletonPtr()->getUIManager()->setUpgradeText(Damage);
		break;
	default:
		Debug("Wrong upgrade type");
		break;
	}
}

/// <summary>
/// Players update loop, updates instance inbetween frames
/// </summary>
/// <param name="pDeltaTime">The delta time, time since last frame.</param>
void Player::update(Ogre::Real pDeltaTime)
{
	Ogre::Vector3 attackSpherePos = Ogre::Vector3(getAttackDistance() * .75f, 0, 0);
	attackSpherePos = Ogre::Quaternion(Ogre::Degree(-std::abs(_currAttackCooldown / _lightAttackCooldown - 0.5f) * 180 + 90), Ogre::Vector3::UNIT_Y) * attackSpherePos;// _currAttackCooldown;
	attackSpherePos.z = attackSpherePos.z * 2;
	attackNode->setPosition(attackSpherePos);

	if (_myNode == nullptr) return;

	_dirVec.x = 0;
	_dirVec.z = 0;
	if (GameManager::getSingletonPtr()->up){
		_dirVec.z = -1;
	}
	if (GameManager::getSingletonPtr()->down){
		_dirVec.z = 1;
	}
	if (GameManager::getSingletonPtr()->left){
		_dirVec.x = -1;
	}
	if (GameManager::getSingletonPtr()->right){
		_dirVec.x = 1;
	}

	// don't do stuff when in dialog
	if (_inDialog) return;

	// set epic music when close to sis
	if (GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getEndCity()->inThisCity(getPosition())) {
		GameManager::getSingletonPtr()->getSoundManager()->triggerEndRoom();
		// if player reached the end 
		if (GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getDistToSis(getPosition()) < _noticeDistance) {
			setSisterNearby(true);
		}
		else {
			setSisterNearby(false);
		}
	}
	else {
		GameManager::getSingletonPtr()->getSoundManager()->triggerEndRoom(false);
		setSisterNearby(false);
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

	for (int i = 0; i < GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getZone(0,0).cities.size(); i++)
	{
		Coordinate position = Coordinate(getPosition().x / Zone::scalar, getPosition().z / Zone::scalar);
		City city = GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getZone(0, 0).cities[i];
		bool inCity = city.inCity(position);
		if (inCity)
		{
			if (curCityId == -1)
			{
				curCityId = GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getZone(0, 0).cities[i].ID();
				std::string cName = city.getName();
				GameManager::getSingletonPtr()->getUIManager()->showHUDText("Welcome to " + cName, 2.5f);
			}
		}
		else if (curCityId == city.ID())
		{
			curCityId = -1;
		}
	}
}

void Player::interactionTriggered() {
	if(_sisNearby) {
		findSister();
	} else if(_nearbyNPC != nullptr) {
		dialogTriggered();
	}
}

void Player::findSister() {
	_myNode = nullptr;
	_myEntity = nullptr;

	GameManager::getSingletonPtr()->goToState(End);
	GameManager::getSingletonPtr()->getSoundManager()->completed();
	GameManager::getSingletonPtr()->getSoundManager()->triggerEndRoom(false);
}

void Player::setSisterNearby(bool val) {
	if (_sisNearby == val) return;

	_sisNearby = val;

	if(_sisNearby) {
		GameManager::getSingletonPtr()->getUIManager()->showHUDText("Press 'E' to talk to your sister.");
	} else {
		GameManager::getSingletonPtr()->getUIManager()->hideHUDText();
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

	if (!_nearbyNPC->talk()) {
		if (_hasItem && _needToGiveItem && _nearbyNPC->relevantForAction()) {
			giveItem(_nearbyNPC);
		} 
		if (_nearbyNPC->relevantForAction()) {
			GameManager::getSingletonPtr()->getQuestManager()->getCurrentQuest()->sendMsg(this, Action::msgNpcInfo);
		}

		_inDialog = false;
		GameManager::getSingletonPtr()->getSoundManager()->exitDialog();
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
	_canAttack = false;
	_currAttackCooldown = _lightAttackCooldown;

	std::vector<Character*> targets = GameManager::getSingletonPtr()->getLevelManager()->getHostileNpcs();
	findTarget(targets);

	if (_target == nullptr) {
		return false;
	}

	//deal damage 
	changeInBattle(true);

	_inBattleTime = 0;

	if (_target->adjustHealth(_damage)) {
		GameManager::getSingletonPtr()->getUIManager()->adjustEnemyHealthBar(_target->getCurrHealth(), _target->getMaxHealth());

	}
	else {
		changeInBattle(false);

	}

	return true;
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

void Player::recieveItem() {
	Character::recieveItem();

	GameManager::getSingletonPtr()->getQuestManager()->getCurrentQuest()->sendMsg(this, Action::msgPlayerItem);
}
