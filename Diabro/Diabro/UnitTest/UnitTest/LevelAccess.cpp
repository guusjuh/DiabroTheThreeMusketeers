#include "LevelAccess.h"

LevelAccess::LevelAccess()
{
}

LevelAccess::LevelAccess(Zone z)
	:_zone(z)
{
}

LevelAccess::~LevelAccess()
{
}

int LevelAccess::getMaxValue()
{
	return _zone.getMaxValue();
}