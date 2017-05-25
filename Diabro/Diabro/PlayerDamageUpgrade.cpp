#include "PlayerDamageUpgrade.h"


PlayerDamageUpgrade::PlayerDamageUpgrade(IPlayerEquipment equipment, float damageMod)
: PlayerUpgrade(equipment, 0, damageMod)
{
}

PlayerDamageUpgrade::~PlayerDamageUpgrade()
{
}

float PlayerDamageUpgrade::getDamage()
{
	return equipment.getDamage() + damageMod;
}

