#ifndef _PLAYER_EQUIPMENT_H_
#define _PLAYER_EQUIPMENT_H_
#include "IPlayerEquipment.h"

struct PlayerUpgradeType {
	int value;
	UpgradeModifierType modifier;
	PlayerUpgradeType() : value(0), modifier((UpgradeModifierType)0) {}
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


