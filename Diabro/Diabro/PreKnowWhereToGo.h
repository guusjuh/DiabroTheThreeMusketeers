#ifndef PRE_KNOW_WHERE_TO_GO
#define PRE_KNOW_WHERE_TO_GO
#include "PreCondition.h"

class PreKnowWhereToGo : public PreCondition {
	friend class Action;
public:
	PreKnowWhereToGo() { }
	~PreKnowWhereToGo() {}

	void start() override;

private:
	IQuestContent* city;
};

#endif
