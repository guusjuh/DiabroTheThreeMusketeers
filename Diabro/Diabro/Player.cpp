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
	_movespeed = 250;
	_runspeed = 450;

	// initialize pLevel vars
	_currentLevel = 1;
	_currentXP = 0;
	_xpTillNextLevel = calcXpTillLevel(_currentLevel + 1);
}

/// <summary>
/// Initializes this instance.
/// </summary>
/// <returns></returns>
bool Player::initialize()
{
	Character::initialize();

	return true;
}

/// <summary>
/// Adjusts the health.
/// </summary>
/// <param name="pAdjust">The adjustment of health.</param>
/// <returns>False if the player runs out of health and dies.</returns>
bool Player::adjustHealth(float pAdjust)
{
	if (!Character::adjustHealth(pAdjust)) { return false; }

	GameManager::getSingleton().getUIManager()->adjustHealthBar(_currentHealth, _stats->GetStat(StatType::MaxHealth));
	return true;
}

/// <summary>
/// Adjusts the stamina over time.
/// </summary>
/// <param name="deltatime">The time since last frame.</param>
/// <returns>False if the player runs out of statina.</returns>
bool Player::adjustStaminaOverTime(Ogre::Real pDeltaTime)
{
	Character::adjustStaminaOverTime(pDeltaTime);

	GameManager::getSingleton().getUIManager()->adjustStaminaBar(_currentStamina, _stats->GetStat(StatType::MaxStamina));

	return true;
}
bool Player::AttackCooldown(bool Cooldown)
{
	_canAttack = Cooldown;
	return true;
}

bool Player::LightAttack(bool Weapon)
{

	FILE* fp;

	freopen_s(&fp, "CONOUT$", "w", stdout);

	//need to make function to calculate distance between the two.
	//damage calculation with playerstats.
	int AttackDamage = _stats->GetStat(Damage) * _stats->GetStat(Strength);
	if (_canAttack)
	{
		//check if no cooldown cd = AttackSpeed
		if (_AttackCD <= 0)
		{
			//deal damage 
			_BasicEnemy->AdjustHealth(AttackDamage, Weapon);
			printf("dealingDamage");
			_canAttack = false;
			//_AttackCD = _stats->GetStat(AttackSpeed) + 10;
		}
	}
	//check if target is in range if distance to mouse pos is less than attack range then attack
	//need to get object under mouse and extract hp from the object.

	return true;
}

/// <summary>
/// Adjusts the stamina.
/// </summary>
/// <param name="pAdjust">The adjustment in stamina.</param>
/// <returns>False if the player runs out of statina.</returns>
bool Player::adjustStamina(float pAdjust)
{
	Character::adjustStamina(pAdjust);

	GameManager::getSingleton().getUIManager()->adjustStaminaBar(_currentStamina, _stats->GetStat(StatType::MaxStamina));

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
