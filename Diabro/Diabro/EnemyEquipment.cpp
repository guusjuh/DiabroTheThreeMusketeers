#include "EnemyEquipment.h"

EnemyEquipment::EnemyEquipment(float health, float damage, float noticeDist) 
	: health(health), damage(damage), noticeDist(noticeDist) { }

EnemyEquipment::~EnemyEquipment() { }

float EnemyEquipment::getHealth()
{
	return health;
}

float EnemyEquipment::getDamage()
{
	return damage;
}

float EnemyEquipment::getNoticeDist() 
{
	return noticeDist;
}
