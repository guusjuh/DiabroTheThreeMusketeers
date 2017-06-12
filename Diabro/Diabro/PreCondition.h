#ifndef PRE_CONDITION_H
#define PRE_CONDITION_H

class PreCondition {
public:
	PreCondition() { _isMet = false; }
	~PreCondition() {}

	virtual void start() {}
	bool isMet() { return _isMet; }

protected:
	bool _isMet;
};

#endif
