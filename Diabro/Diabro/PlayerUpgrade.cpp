#include "PlayerUpgrade.h"


PlayerUpgrade::PlayerUpgrade(IPlayerEquipment base, float healthMod = 0, float damageMod = 0)
	: equipment(base), healthMod(healthMod), damageMod(damageMod)
{
}

PlayerUpgrade::~PlayerUpgrade()
{
}

bool PlayerUpgrade::IsBase()
{
	return false;
}

float PlayerUpgrade::getHealth()
{
	return equipment.getHealth() + healthMod;
}

float PlayerUpgrade::getDamage()
{
	return equipment.getDamage() + damageMod;
}



