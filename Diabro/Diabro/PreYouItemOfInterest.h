#ifndef PRE_YOU_ITEM_OF_INTEREST
#define PRE_YOU_ITEM_OF_INTEREST
#include "PreCondition.h"

class PreYouItemOfInterest : public PreCondition {
	friend class Action;
public:
	PreYouItemOfInterest() { }
	~PreYouItemOfInterest() {}

	void start() override;

private:
	IQuestContent* item;
};

#endif
