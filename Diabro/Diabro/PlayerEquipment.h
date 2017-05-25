#ifndef _PLAYER_EQUIPMENT_H_
#define _PLAYER_EQUIPMENT_H_
#include "IPlayerEquipment.h"

class PlayerEquipment : public IPlayerEquipment
{
public:
	PlayerEquipment(float health, float damage);
	~PlayerEquipment();

	float getDamage() override;
	float getHealth() override;

	bool IsBase() override{ return true; }

private:
	float health;
	float damage;
};
#endif


