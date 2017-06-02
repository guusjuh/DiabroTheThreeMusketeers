#ifndef BASE_ENEMY_H_
#define BASE_ENEMY_H_

#include "BaseNpc.h"
#include "StateMachine.h"
#include "IdleState.h"
#include "EnemyEquipment.h"

/// <summary>
/// The basic class for an enemy.
/// </summary>
/// <seealso cref="BaseNpc" />
class BasicEnemy : public BaseNpc
{
public:
	BasicEnemy(Ogre::SceneNode* pMyNode, Ogre::SceneNode* pMyRotationNode, Ogre::Entity* pMyEntity, City* pMyCity, int level);
	~BasicEnemy() {}

	void update(Ogre::Real) override;
	void die() override;
	void updateBar(bool val) { _updateBar = val; }

	StateMachine<Character> stateMachine;

private:
	bool lightAttack() override;
	bool _updateBar;

	void assignUpgrades(int level);
	void upgradeEquipment(EnemyUpgradeType upgrade);
	IEnemyEquipment* equipment;

	int healthUpgrades, damageUpgrades, noticeDistUpgrades;

	static const int LOW_HP;
	static const int HIGH_HP;
	static const int LOW_DMG;
	static const int HIGH_DMG;
	static const int LOW_NDIST;
	static const int HIGH_NDIST;

	static const Ogre::ColourValue COL_HP;
	static const Ogre::ColourValue COL_DMG;
	static const Ogre::ColourValue COL_NDIST;
};

#endif
