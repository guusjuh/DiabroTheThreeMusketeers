#pragma once
class IDecorator
{
public:
	IDecorator();
	~IDecorator();

	virtual bool IsBase();
	virtual IDecorator removeUpgrades();
};

