#ifndef ACTION_H
#define ACTION_H

enum ActionType {
	Export = 0,
	Exchange,
	Explore,
	Get,
	Give,
	Goto,
	Kill,
	Listen,
	Report,
};

enum PreconditionsType {
	SomebodyThere = 0,
	SomethingThere,
	KnowWhereToGo,
	YouItemOfInterest,
	TheyItemOfInterest,
	BothItemOfInterest,
};

enum PostconditionType {
	Accompanied,
	YouHaveTheItem,
	TheyHaveTheItem,
	BothHaveTheItem,
	YouInfo,
	TheyInfo,
	YouThere,
	TheyDead,
};

class Action {
public:
	Action(){}
	~Action(){}

	ActionType GetType() { return _type; }
	std::vector<PreconditionsType> GetPreconditions() { return _preconditions; }
	PostconditionType GetPostcondition() { return _postcondition; }
	std::map<QuestContent, int> GetRequiredContent() { return _requiredContent; }

private:
	ActionType _type;

	std::vector<PreconditionsType> _preconditions;
	PostconditionType _postcondition;

	std::map<QuestContent, int> _requiredContent;
};

#endif