#ifndef _ENEMY_EQUIPMENT_H_
#define _ENEMY_EQUIPMENT_H_
#include "IEnemyEquipment.h"

struct EnemyUpgradeType {
	int value;
	UpgradeModifierType modifier;
	EnemyUpgradeType(int v, UpgradeModifierType t) : value(v), modifier(t) {}
};

class EnemyEquipment : public IEnemyEquipment
{
public:
	EnemyEquipment(float health, float damage, float noticeDist);
	~EnemyEquipment();

	float getDamage() override;
	float getHealth() override;
	float getNoticeDist() override;

	bool isBase() override { return true; }
	IEnemyEquipment* getBase() override { return nullptr; }
	UpgradeModifierType getType() override { return None; }
	IEnemyEquipment* removeUpgrades() override { return this; }

private:
	float health;
	float damage;
	float noticeDist;
};
#endif


