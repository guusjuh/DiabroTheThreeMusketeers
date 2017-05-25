#ifndef IPLAYER_EQUIPMENT_H_
#define IPLAYER_EQUIPMENT_H_

#include "IDecorator.h"

class IPlayerEquipment : public IDecorator
{
public:
	virtual float getHealth();
	virtual float getDamage();

	using IDecorator::IsBase;
	// enemy noticerange (met een max)
};
#endif
