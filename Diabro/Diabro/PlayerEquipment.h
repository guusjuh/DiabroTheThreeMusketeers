#ifndef _PLAYER_EQUIPMENT_H_
#define _PLAYER_EQUIPMENT_H_
#include "IPlayerEquipment.h"

enum UpgradeModifierType {
	Health,
	Damage
};

struct PlayerUpgradeType {
	int value;
	UpgradeModifierType modifier;
	PlayerUpgradeType(int v, UpgradeModifierType t) : value(v), modifier(t) {}
};

class PlayerEquipment : public IPlayerEquipment
{
public:
	PlayerEquipment(float health, float damage);
	~PlayerEquipment();

	float getDamage() override;
	float getHealth() override;

	bool isBase() override{ return true; }

	IPlayerEquipment* removeUpgrades() override { return this; }

private:
	float health;
	float damage;
};
#endif


