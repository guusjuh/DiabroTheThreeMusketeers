#ifndef PRE_CONDITION_H
#define PRE_CONDITION_H
#include <vector>
#include "IQuestContent.h"

//TODO: types to implement:
// 1. something there
// 2. somebody there
// 3. you have item of interest
// 4. know where to go

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
