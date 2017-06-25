#ifndef BASE_ENEMY_H_
#define BASE_ENEMY_H_

#include "BaseNpc.h"
#include "EnemyEquipment.h"

#include "EnemyFollowAStarState.h"
#include "EnemyFollowDirectState.h"
#include "EnemyMeleeAttackState.h"
#include "EnemyMoveAroundCenterState.h"
#include "EnemyWalkToPointNearPlayerState.h"
#include "EnemyChargeState.h"

/// <summary>
/// The basic class for an enemy.
/// </summary>
/// <seealso cref="BaseNpc" />
class BaseEnemy : public BaseNpc
{
public:
	BaseEnemy(Ogre::SceneNode*, Ogre::SceneNode*, Ogre::Entity*, City*, int);
	~BaseEnemy() {}

	virtual void collide() override;
	virtual void update(Ogre::Real) override;
	void die() override;
	virtual bool lightAttack() override;
	void updateBar(bool val) { _updateBar = val; }

	QuestContent getType() override { return EnemyQC; }
	std::string getName() { return name; }

protected:
	bool _initialized;
	bool _updateBar;

	static const float killRestorePercentage;

	int healthUpgrades, damageUpgrades, noticeDistUpgrades;
	IEnemyEquipment* equipment;
	void assignUpgrades(int level);
	void upgradeEquipment(EnemyUpgradeType upgrade);
	UpgradeModifierType getMostUsedUpgrade();

	std::string name;
	std::vector<std::string> getNameOptions();
	
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
