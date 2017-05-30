#pragma once

enum UpgradeModifierType {
	None,
	Health,
	Damage,
	NoticeDist,
	AMOUNT_OF_UPGRADE_TYPES
};

class IDecorator
{
public:
	IDecorator() {}
	~IDecorator() {}

	virtual bool isBase() { return false; }
};

