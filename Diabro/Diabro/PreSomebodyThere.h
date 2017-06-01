#ifndef PRE_SOMEBODY_THERE_H
#define PRE_SOMEBODY_THERE_H
#include "PreCondition.h"

class PreSomebodyThere : public PreCondition {
	friend class Action;
public:
	PreSomebodyThere() { }
	~PreSomebodyThere() {}

	void start() override;

private:
	std::vector<IQuestContent*> characters;
	IQuestContent* city;
};

#endif
