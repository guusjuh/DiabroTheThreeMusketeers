#pragma once
#include "PlayerUpgrade.h"
#include "IPlayerEquipment.h"

class PlayerDamageUpgrade : PlayerUpgrade
{
public:
	PlayerDamageUpgrade(IPlayerEquipment equipment, float damageMod);
	~PlayerDamageUpgrade();

	float getDamage() override;
};

