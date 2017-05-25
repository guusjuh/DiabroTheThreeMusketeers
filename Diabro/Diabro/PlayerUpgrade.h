#pragma once
#include "IPlayerEquipment.h"

class PlayerUpgrade : IPlayerEquipment
{
public:
	PlayerUpgrade(IPlayerEquipment base, float healthMod = 0, float damageMod = 0);
	~PlayerUpgrade();

	float healthMod;
	float damageMod;

	float getHealth() override;
	float getDamage() override;
	bool IsBase() override;
protected:
	IPlayerEquipment equipment;
};

