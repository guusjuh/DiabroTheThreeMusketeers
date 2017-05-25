#ifndef ACTION_H
#define ACTION_H

#include <vector>
#include <map>
#include "QuestContentManager.h"

enum ActionType {
	Escort = 0,
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
	/// <summary>
	/// Initializes a new instance of the <see cref="Action"/> class.
	/// </summary>
	Action() : _id(0), _type((ActionType)0), _preconditions(0), _postcondition((PostconditionType)0) {}

	/// <summary>
	/// Initializes a new instance of the <see cref="Action"/> class.
	/// </summary>
	/// <param name="pID">The id.</param>
	/// <param name="pType">Type of the action.</param>
	/// <param name="pPreconditions">The preconditions.</param>
	/// <param name="pPostcondition">The postcondition.</param>
	/// <param name="pQuestContent">Required content for this action.</param>
	Action(int pID, ActionType pType, std::vector<PreconditionsType> pPreconditions, PostconditionType pPostcondition, std::map<QuestContent, int> pQuestContent) :
	_id(pID), _type(pType), _preconditions(pPreconditions), _postcondition(pPostcondition), _requiredContent(pQuestContent) {}

	/// <summary>
	/// Finalizes an instance of the <see cref="Action"/> class.
	/// </summary>
	~Action() {}

	int getID() { return _id; }
	ActionType getType() { return _type; }
	std::vector<PreconditionsType> getPreconditions() { return _preconditions; }
	PostconditionType getPostcondition() { return _postcondition; }
	std::map<QuestContent, int> getRequiredContent() { return _requiredContent; }

	bool completed() { return _completed; }
	void complete() { if (!_completed) _completed = true; }

private:
	int _id;
	ActionType _type;

	std::vector<PreconditionsType> _preconditions;
	PostconditionType _postcondition;

	std::map<QuestContent, int> _requiredContent;

	bool _completed;
};

#endif