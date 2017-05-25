#include "PlayerHealthUpgrade.h"


PlayerHealthUpgrade::PlayerHealthUpgrade(IPlayerEquipment equipment, float healthMod)
	:PlayerUpgrade(equipment, healthMod)
{
}

PlayerHealthUpgrade::~PlayerHealthUpgrade()
{
}

float PlayerHealthUpgrade::getHealth()
{
	return equipment.getHealth() + healthMod;
}
