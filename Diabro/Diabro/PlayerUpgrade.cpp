#include "PlayerUpgrade.h"


PlayerUpgrade::PlayerUpgrade(IPlayerEquipment* base, float healthMod, float damageMod)
	: equipment(base), healthMod(healthMod), damageMod(damageMod)
{
}

PlayerUpgrade::~PlayerUpgrade()
{
}

bool PlayerUpgrade::isBase()
{
	return false;
}

float PlayerUpgrade::getHealth()
{
	return equipment->getHealth() + healthMod;
}

float PlayerUpgrade::getDamage()
{
	return equipment->getDamage() + damageMod;
}

IPlayerEquipment* PlayerUpgrade::removeUpgrades() {
	if (equipment->isBase()) {
		return equipment;
	}
	else {
		return equipment->removeUpgrades();
	}
}
