#ifndef ACTION_H
#define ACTION_H

#include <vector>
#include <map>
#include "QuestContentManager.h"

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
	/// <summary>
	/// Initializes a new instance of the <see cref="Action"/> class.
	/// </summary>
	Action() : _id(0), _type((ActionType)0), _preconditions(0), _postcondition((PostconditionType)0), _requiredContent(), _concreteContent(0) {
		_completed = false;
	}

	/// <summary>
	/// Initializes a new concrete instance of the <see cref="Action"/> class.
	/// </summary>
	/// <param name="pID">The id.</param>
	/// <param name="pType">Type of the action.</param>
	/// <param name="pPreconditions">The preconditions.</param>
	/// <param name="pPostcondition">The postcondition.</param>
	/// <param name="pQuestContent">Required content for this action.</param>
	Action(int pID, ActionType pType, std::vector<PreconditionsType> pPreconditions, PostconditionType pPostcondition, std::vector<QuestContent> pQuestContent, std::vector<IQuestContent*> pConcreteContent) :
	_id(pID), _type(pType), _preconditions(pPreconditions), _postcondition(pPostcondition) {
		std::vector<std::pair<QuestContent, int>> tempRequiredContent;

		for (int i = 0; i < pQuestContent.size(); ++i) {
			tempRequiredContent.push_back(std::pair<QuestContent, int>(pQuestContent[i], 0));
		}
		
		_requiredContent = tempRequiredContent;

		std::vector<std::pair<IQuestContent*, int>> tempConcreteContent;

		for (int i = 0; i < pQuestContent.size(); ++i) {
			tempConcreteContent.push_back(std::pair<IQuestContent*, int>(pConcreteContent[i], 0));
		}

		_concreteContent = tempConcreteContent;

		_completed = false;
	} 

	/// <summary>
	/// Initializes a new abstract instance of the <see cref="Action"/> class.
	/// </summary>
	/// <param name="pID">The id.</param>
	/// <param name="pType">Type of the action.</param>
	/// <param name="pPreconditions">The preconditions.</param>
	/// <param name="pPostcondition">The postcondition.</param>
	/// <param name="pQuestContent">Required content for this action.</param>
	Action(int pID, ActionType pType, std::vector<PreconditionsType> pPreconditions, PostconditionType pPostcondition, std::vector<QuestContent> pQuestContent) :
		_id(pID), _type(pType), _preconditions(pPreconditions), _postcondition(pPostcondition), _concreteContent(0) {
		std::vector<std::pair<QuestContent, int>>	tempRequiredContent;

		for (int i = 0; i < pQuestContent.size(); ++i) {
			tempRequiredContent.push_back(std::pair<QuestContent, int>(pQuestContent[i], 0));
		}

		_requiredContent = tempRequiredContent;

		_completed = false;
	}

	/// <summary>
	/// Initializes a new abstract instance of the <see cref="Action"/> class.
	/// </summary>
	/// <param name="pID">The id.</param>
	/// <param name="pType">Type of the action.</param>
	/// <param name="pPreconditions">The preconditions.</param>
	/// <param name="pPostcondition">The postcondition.</param>
	/// <param name="pQuestContent">Required content for this action.</param>
	Action(int pID, ActionType pType, std::vector<PreconditionsType> pPreconditions, PostconditionType pPostcondition, std::vector<std::pair<QuestContent, int>> pQuestContent) :
		_id(pID), _type(pType), _preconditions(pPreconditions), _postcondition(pPostcondition), _requiredContent(pQuestContent), _concreteContent(0) {
		_completed = false;
	}

	/// <summary>
	/// Finalizes an instance of the <see cref="Action"/> class.
	/// </summary>
	~Action() {}

	int getID() { return _id; }
	ActionType getType() { return _type; }

	std::vector<PreconditionsType> getPreconditions() { return _preconditions; }
	PostconditionType getPostcondition() { return _postcondition; }

	std::vector<std::pair<QuestContent, int>> getRequiredContent() { return _requiredContent; }
	std::vector<QuestContent> getRequiredContentTypes() {
		std::vector<QuestContent> returnVector;
		for (int i = 0; i < getRequiredContent().size(); ++i) {
			returnVector.push_back(_requiredContent[i].first);
		}
		return returnVector;
	}
	std::vector<std::pair<IQuestContent*, int>> getConcreteContent() { return _concreteContent; }

	bool isAbstract() { return getConcreteContent().size() == 0 ? true : false; }

	bool completed() { return _completed; }
	void complete() { if (!_completed) _completed = true; }

private:
	int _id;
	ActionType _type;

	std::vector<PreconditionsType> _preconditions;
	PostconditionType _postcondition;

	std::vector<std::pair<QuestContent, int>> _requiredContent;
	std::vector<std::pair<IQuestContent*, int>> _concreteContent;

	bool _completed;
};

#endif