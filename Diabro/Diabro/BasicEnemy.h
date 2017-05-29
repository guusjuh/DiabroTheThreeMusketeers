#ifndef BASE_ENEMY_H_
#define BASE_ENEMY_H_

#include "BaseNpc.h"
#include "StateMachine.h"
#include "IdleState.h"
#include "WanderState.h"
#include "EnemyFollowState.h"
#include "EnemyAttackState.h"
#include "EnemyMoveAroundCenterState.h"
#include "EnemyEquipment.h"

/// <summary>
/// The basic class for an enemy.
/// </summary>
/// <seealso cref="BaseNpc" />
class BasicEnemy : public BaseNpc
{
public:
	BasicEnemy(Ogre::SceneNode*, Ogre::SceneNode*, Ogre::Entity*, City*, int);
	~BasicEnemy() {}

	void update(Ogre::Real) override;
	void die() override;
	void updateBar(bool val) { _updateBar = val; }

	bool lightAttack() override;

private:
	bool _updateBar;

	void assignUpgrades(int level);
	void upgradeEquipment(EnemyUpgradeType upgrade);
	IEnemyEquipment* equipment;

	int healthUpgrades, damageUpgrades, noticeDistUpgrades;

	std::map<std::string, State<BaseNpc>*> possibleStates;
	bool _initialized;

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
