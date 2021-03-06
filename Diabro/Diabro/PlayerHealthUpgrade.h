#pragma once
#include "PlayerUpgrade.h"

class PlayerHealthUpgrade : public PlayerUpgrade
{
public:
	PlayerHealthUpgrade(IPlayerEquipment* equipment, float healthMod);
	~PlayerHealthUpgrade();

	float getHealth() override;

	std::string getSprName() override { return "UI/upgradeHp"; }
};

