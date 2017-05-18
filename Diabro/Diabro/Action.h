#ifndef ACTION_H
#define ACTION_H

#include <vector>
#include <map>
#include "QuestContentManager.h"

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
	Action() : _id(0), _type((ActionType)0), _preconditions(0), _postcondition((PostconditionType)0){}
	Action(int pID, ActionType pType, std::vector<PreconditionsType> pPreconditions, PostconditionType pPostcondition) :
	_id(pID), _type(pType), _preconditions(pPreconditions), _postcondition(pPostcondition) {}
	~Action(){}

	ActionType GetType() { return _type; }
	std::vector<PreconditionsType> GetPreconditions() { return _preconditions; }
	PostconditionType GetPostcondition() { return _postcondition; }
	std::map<QuestContent, int> GetRequiredContent() { return _requiredContent; }

private:
	int _id;
	ActionType _type;

	std::vector<PreconditionsType> _preconditions;
	PostconditionType _postcondition;

	std::map<QuestContent, int> _requiredContent;
};

#endif