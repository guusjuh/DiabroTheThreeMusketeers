#ifndef PRE_CONCRETE_PRE_CONDITIONS
#define PRE_CONCRETE_PRE_CONDITIONS
#include "PreCondition.h"
#include "IQuestContent.h"

class PreYouItemOfInterest : public PreCondition {
	friend class Action;
public:
	PreYouItemOfInterest() { }
	~PreYouItemOfInterest() {}

	void start() override;

private:
	IQuestContent* item;
};

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