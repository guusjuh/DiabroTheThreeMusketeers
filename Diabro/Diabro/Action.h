#ifndef ACTION_H
#define ACTION_H

#include <vector>
#include <map>
#include "QuestContentManager.h"
#include "PreCondition.h"
#include "PostCondition.h"

/// <summary>
/// The types of actions.
/// </summary>
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

/// <summary>
/// The types of preconditions.
/// </summary>
enum PreconditionsType {
	SomebodyThere = 0,
	SomethingThere,
	KnowWhereToGo,
	YouItemOfInterest,
	TheyItemOfInterest,
	BothItemOfInterest,
};

/// <summary>
/// The types of postconditions.
/// </summary>
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

/// <summary>
/// An action to be in a quest. 
/// </summary>
class Action { 
	// my bffs <3
	friend class StrategyContainer;
	friend class Strategy;
	friend class QuestGenerator;

public:
	Action();
	Action(int pID, ActionType pType, std::vector<PreconditionsType> pPreconditions, PostconditionType pPostcondition, std::vector<QuestContent> pQuestContent, std::string dialog);
	Action(int pID, ActionType pType, std::vector<PreconditionsType> pPreconditions, PostconditionType pPostcondition, std::vector<std::pair<QuestContent, int>> pQuestContent, std::string dialog);
	~Action();

	void start();
	void update();
	void end();

	int getID() { return _id; }
	ActionType getType() { return _type; }
	std::string getDialog() { return _dialog; }
	std::vector<PreconditionsType> getPreconditions() {
		std::vector<PreconditionsType> returnVector;

		std::map<PreconditionsType, PreCondition*>::iterator it;
		for (it = _preconditions.begin(); it != _preconditions.end(); it++) {
			returnVector.push_back(it->first);
		}

		return returnVector;
	}
	PostconditionType getPostcondition() { return _postcondition.first; }
	std::vector<std::pair<QuestContent, int>> getRequiredContent() { return _requiredContent; }
	std::vector<std::pair<IQuestContent*, int>> getConcreteContent() { return _concreteContent; }

	std::vector<QuestContent> getRequiredContentTypes();
	IQuestContent* getTarget();
	int contentContains(QuestContent type);

	bool isAbstract() { return getConcreteContent().size() == 0 ? true : false; }

	bool completed() { return _completed; }
	void complete() { if (!_completed) _completed = true; }

	static const std::string msgCityReached;
	static const std::string msgPlayerItem;
	static const std::string msgNpcItem;

	void sendMsg(std::string msg);

private:

	int _id;
	ActionType _type;

	std::map<PreconditionsType, PreCondition*> _preconditions;
	std::pair<PostconditionType, PostCondition*> _postcondition;
	
	void createPreConditions(std::vector<PreconditionsType> precontypes);
	void setPreConditionsContent();

	void createPostConditions(PostconditionType precontype);
	void setPostConditionsContent();

	std::vector<std::pair<QuestContent, int>> _requiredContent;
	std::vector<std::pair<IQuestContent*, int>> _concreteContent;

	std::string _dialog;

	bool _completed;
};

#endif