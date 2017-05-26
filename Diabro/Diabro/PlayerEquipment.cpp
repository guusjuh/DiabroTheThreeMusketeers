#include "PlayerEquipment.h"

PlayerEquipment::PlayerEquipment(float health, float damage)
	: health(health), damage(damage)
{

}


PlayerEquipment::~PlayerEquipment()
{

}

float PlayerEquipment::getHealth()
{
	return health;
}

float PlayerEquipment::getDamage()
{
	return damage;
}

