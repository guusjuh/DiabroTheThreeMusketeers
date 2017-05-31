#ifndef PRE_SOMETHING_THERE_H
#define PRE_SOMETHING_THERE_H
#include "PreCondition.h"

class PreSomethingThere : public PreCondition {
	friend class Action;
public:
	PreSomethingThere() { }
	~PreSomethingThere() {}

	void start() override;

private: 
	IQuestContent* character;
	IQuestContent* item;
};

#endif
