#ifndef STRATEGY_H
#define STRATEGY_H

#pragma once
#include "Needs.h"
#include "Action.h"
#include "QuestContentManager.h"

struct QuestName {
	std::string preString;
	QuestContent templateContent;
	std::string postString;
};

/// <summary>
/// A strategy forms the basis for a quest by defining it's actions and content.
/// </summary>
class Strategy {
public:
	/// <summary>
	/// Initializes a new instance of the <see cref="Strategy"/> class.
	/// </summary>
	/// <param name="pid">The id of the strategy.</param>
	/// <param name="pName">Name of the strategy.</param>
	/// <param name="pNameQuest">The template name of the quest.</param>
	/// <param name="pMotivation">The motivation for the quest.</param>
	/// <param name="pActions">The actions that must be completed for the quest.</param>
	Strategy(int pid, std::string pName, QuestName pNameQuest, NeedType pMotivation, std::vector<Action> pActions, int pRarityPref)
		: _id(pid), _nameStrategy(pName), _nameQuest(pNameQuest), _motivation(pMotivation), _actionSequence(pActions), _rarityPref(pRarityPref) {
		_currentAction = 0;
	}

	/// <summary>
	/// Initializes a new instance of the <see cref="Strategy"/> class.
	/// </summary>
	Strategy()
		: _id(0), _nameStrategy(""), _nameQuest(), _motivation((NeedType)0), _actionSequence(0), _rarityPref(0) {
		_currentAction = -1;
	}

	/// <summary>
	/// Finalizes an instance of the <see cref="Strategy"/> class.
	/// </summary>
	~Strategy() {
		_actionSequence.clear();
	}

	int getID() { return _id; }
	std::string getName() { return _nameStrategy; }
	QuestName getNameQuest() { return _nameQuest; }
	NeedType getMotivation() { return _motivation; }

	std::vector<Action> getActionSequence() {
		return _actionSequence;
	}
	Action* getCurrentAction() {
		return &_actionSequence[_currentAction];
	}

	void increaseAction() {
		_currentAction++;
		/*
		// find which action is the current
		for (int i = 0; i < getActionSequence().size(); ++i) {

			// if we found the current
			if (_currrentAction == &getActionSequence()[i]) {
				_currrentAction = &getActionSequence()[i + 1];
				return;
			}
		}*/
	}

	int getRarityPref() { return _rarityPref; }

	bool isAbstract() { return _actionSequence[0].getConcreteContent().size() == 0 ? true : false; }

private:
	int _id;
	std::string _nameStrategy;
	QuestName _nameQuest;
	NeedType _motivation;

	std::vector<Action> _actionSequence;
	int _currentAction;

	int _rarityPref;
};

#endif 
