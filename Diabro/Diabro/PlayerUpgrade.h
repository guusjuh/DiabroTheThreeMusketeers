#pragma once
#include "IPlayerEquipment.h"
#include <string>

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

	virtual std::string getSprName() { return ""; }

protected:
	IPlayerEquipment* equipment;

	IPlayerEquipment* removeUpgrades() override;
};

