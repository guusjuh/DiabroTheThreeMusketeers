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

	_lightAttackCooldown = 1.2f;
}

void Player::update(Ogre::Real deltaTime) 
{
	if (_inDialog) return;

	Character::update(deltaTime);

	for (int i = 0; i < GameManager::getSingletonPtr()->getLevelManager()->getFriendlyNpcs().size(); ++i) {
		if (getPosition().distance(GameManager::getSingletonPtr()->getLevelManager()->getFriendlyNpcs()[i]->getPosition()) < _noticeDistance) {
			_nearbyNPC = dynamic_cast<Npc*>(GameManager::getSingletonPtr()->getLevelManager()->getFriendlyNpcs()[i]);
			break;
		}
		_nearbyNPC = nullptr;
	}

	if (_nearbyNPC != nullptr) {
		GameManager::getSingletonPtr()->getUIManager()->showHUDText("Press 'E' to talk.");
	}
	else {
		GameManager::getSingletonPtr()->getUIManager()->hideHUDText();
	}
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
