#ifndef IPLAYER_EQUIPMENT_H_
#define IPLAYER_EQUIPMENT_H_

#include "IDecorator.h" 

class IPlayerEquipment : public IDecorator
{
public:
	virtual float getHealth() { return 0; }
	virtual float getDamage() { return 0; }

	using IDecorator::isBase;
	virtual IPlayerEquipment* removeUpgrades() { return nullptr; }
	// enemy noticerange (met een max)
};
#endif
