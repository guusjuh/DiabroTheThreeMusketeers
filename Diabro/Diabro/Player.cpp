#include "Player.h"
#include "GameManager.h"

/// <summary>
/// Creates a new instance of the <see cref="Player"/> class.
/// </summary>
/// <param name="pMyNode">My node.</param>
/// <param name="pMyEntity">My entity.</param>
Player::Player(Ogre::SceneNode* pMyNode, Ogre::Entity* pMyEntity) : Character(pMyNode, pMyEntity)
{
	// override default speeds
	_movespeed = 450;

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

void Player::reset(Ogre::SceneNode* pMyNode, Ogre::Entity* pMyEntity) {
	_myNode = pMyNode;
	_myEntity = pMyEntity;

	setDirVector(Ogre::Vector3(0, 0, 0));

	_target = nullptr;
	_inDialog = false;
	_nearbyNPC = nullptr;

	_inBattle = false;
	_inBattleTime = 0;
}

void Player::die() {
	changeInBattle(false);
	GameManager::getSingletonPtr()->goToState(GameState::Died);

	reset(_myNode, _myEntity);

	_myNode->setPosition(GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getStartPos());

	_currentHealth = _maxHealth;
	GameManager::getSingleton().getUIManager()->adjustHealthBar(_currentHealth, _maxHealth);
}

void Player::update(Ogre::Real deltaTime) 
{
	// don't do stuff when in dialog
	if (_inDialog) return;

	// if player reached the end 
	if(GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getDistToSis(getPosition()) < _noticeDistance) {
		GameManager::getSingletonPtr()->goNextState();
	}

	// call basic update
	Character::update(deltaTime);

	if(_inBattle) {
		_inBattleTime += deltaTime;

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

	//angleBetween(Ogre::Vector3(0,0,0));
}

void Player::dialogTriggered() {
	if (_nearbyNPC == nullptr) return;

	_inDialog = true;

	if (!_nearbyNPC->talk(getPosition())) {
		_inDialog = false;
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
	_target->adjustHealth(_damage);
	changeInBattle(true);
	if (_target->getCurrHealth() > 0) {
		GameManager::getSingletonPtr()->getUIManager()->adjustEnemyHealthBar(_target->getCurrHealth(), _target->getMaxHealth());
	} else {
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

void Player::setNearbyNPC(Npc* newNPC) {
	if (_nearbyNPC == newNPC) return;

	_nearbyNPC = newNPC;

	if (_nearbyNPC != nullptr) {
		GameManager::getSingletonPtr()->getUIManager()->showHUDText("Press 'E' to talk.");
	}
	else {
		GameManager::getSingletonPtr()->getUIManager()->hideHUDText();
	}
}

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

void Player::toggleInQuest() {
	_inQuest = !_inQuest; 
	GameManager::getSingletonPtr()->getUIManager()->setQuestOn(_inQuest);
}
