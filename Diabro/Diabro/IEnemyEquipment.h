#ifndef IENEMY_EQUIPMENT_H_
#define IENEMY_EQUIPMENT_H_

#include "IDecorator.h" 

class IEnemyEquipment : public IDecorator
{
public:
	virtual float getHealth() { return 0; }
	virtual float getDamage() { return 0; }
	virtual float getNoticeDist() { return 0; }

	using IDecorator::isBase;	

	virtual IEnemyEquipment* getBase() { return nullptr; }
	virtual UpgradeModifierType getType() { return None; }

	virtual IEnemyEquipment* removeUpgrades() { return nullptr; }
};
#endif
