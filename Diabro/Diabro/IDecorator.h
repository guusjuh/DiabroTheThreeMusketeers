#pragma once
class IDecorator
{
public:
	IDecorator() {}
	~IDecorator() {}

	virtual bool isBase() { return false; }
};

