#pragma once
#include "IPlayerEquipment.h"

class PlayerUpgrade : public IPlayerEquipment
{
public:
	PlayerUpgrade(IPlayerEquipment* base, float healthMod = 0, float damageMod = 0);
	~PlayerUpgrade();

	float healthMod;
	float damageMod;

	float getHealth() override;
	float getDamage() override;
	bool isBase() override;
protected:
	IPlayerEquipment* equipment;
	IPlayerEquipment* removeUpgrades() override;
};

