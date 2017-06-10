#ifndef STRATEGY_H
#define STRATEGY_H

#pragma once
#include "Needs.h"
#include "Action.h"
#include "QuestContentManager.h"

/// <summary>
/// A strategy forms the basis for a quest by defining it's actions and content.
/// </summary>
class Strategy {
	friend class QuestGenerator;
	friend class Quest;
public:
	/// <summary>
	/// Initializes a new instance of the <see cref="Strategy"/> class.
	/// </summary>
	/// <param name="pid">The id of the strategy.</param>
	/// <param name="pName">Name of the strategy.</param>
	/// <param name="pNameQuest">The template name of the quest.</param>
	/// <param name="pMotivation">The motivation for the quest.</param>
	/// <param name="pActions">The actions that must be completed for the quest.</param>
	Strategy(int pid, std::string pName, std::string pNameQuest, std::string pStartDialog, NeedType pMotivation, std::vector<Action> pActions, int pRarityPref)
		: _id(pid), _nameStrategy(pName), _nameQuest(pNameQuest), _startDialog(pStartDialog), _motivation(pMotivation), _actionSequence(pActions), _rarityPref(pRarityPref) {
		_currentAction = -1;
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
	std::string getNameQuest() { return _nameQuest; }
	std::string getDialog() { return _startDialog; }
	NeedType getMotivation() { return _motivation; }
	int getRarityPref() { return _rarityPref; }

	std::vector<Action> getActionSequence() {
		return _actionSequence;
	}
	Action* getCurrentAction() {
		return &_actionSequence[_currentAction];
	}

	bool isAbstract() { return _actionSequence[0].getConcreteContent().size() == 0 ? true : false; }

	void abandon() {
		// end the current action
		if (_currentAction >= 0) {
			_actionSequence[_currentAction].abandon();
		}

		_currentAction = -1;
	}

private:
	int _id;
	std::string _nameStrategy;
	std::string _nameQuest;
	NeedType _motivation;
	int _rarityPref;

	std::string _startDialog;

	std::vector<Action> _actionSequence;
	int _currentAction;

	bool increaseAction() {
		// finish current
		if(_currentAction >= 0) {
			_actionSequence[_currentAction].end();
		}

		// set next
		_currentAction++;

		if(_currentAction >= _actionSequence.size()) {
			return false;
		}

		// start new current
		_actionSequence[_currentAction].start();

		return true;
	}
};

#endif 
