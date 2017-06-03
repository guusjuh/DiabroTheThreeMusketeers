#pragma once
#include "Zone.h"

class LevelAccess
{
public:
	LevelAccess();
	LevelAccess(Zone z);
	~LevelAccess();

	int getMaxValue();

private:
	Zone _zone;
};

